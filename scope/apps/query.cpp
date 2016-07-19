/*
 * Copyright 2015-2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scope/apps/query.h"
#include "scope/apps/container.h"
#include "scope/apps/config.h"
#include "scope/apps/localization.h"
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>
#include <unity/scopes/Department.h>
#include <unity/scopes/OptionSelectorFilter.h>
#include <unity/scopes/FilterOption.h>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QFile>
#include <QTextStream>

namespace usc = unity::scopes;

namespace
{
static const auto ROOT_DEPT_TITLE = _("Desktop Apps");
static const auto HIDDEN_DEPT_TITLE = _("Hidden Desktop Apps");
static const auto DESCRIPTION_FIELD = "description";
static const auto APP_ID_FIELD = "app_id";
static const auto DEPARTMENT_ID_FIELD = "department_id";
static const auto EXCLUDED_APPS_FILTER_TITLE = _("Exclude Apps: ");

struct AppInfo
{
  QString container;
  QString app_id;
  QString key;
};

static AppInfo
parse_app_info(std::string const& uri)
{
  QStringList uri_split = QString::fromStdString(uri).split("/");
  if (uri_split.size() < 4)
  {
    return  AppInfo{};
  }

  return AppInfo{uri_split[2], uri_split[3], QString("%1/%2").arg(uri_split[2]).arg(uri_split[3])};
}

static void
register_departments(usc::SearchReplyProxy const& reply)
{
  usc::CannedQuery departments("libertine-scope.ubuntu");
  departments.set_department_id(ROOT_DEPT_ID);
  departments.set_department_id(HIDDEN_DEPT_ID);

  usc::Department::SPtr root_dept{std::move(usc::Department::create("", departments, ROOT_DEPT_TITLE))};
  root_dept->add_subdepartment(std::move(usc::Department::create(HIDDEN_DEPT_ID, departments, HIDDEN_DEPT_TITLE)));

  reply->register_departments(root_dept);
}


static const auto CATEGORY_APPS_DISPLAY = R"(
    {
        "schema-version" : 1,
        "template" : {
            "category-layout" : "grid",
            "card-size": "small"
        },
        "components" : {
            "title" : "title",
            "art" : {
                "field": "art",
                "fill-mode": "fit",
                "fallback": "image://theme/placeholder-app-icon"
            }
        }
    }
)";


static const auto CATEGORY_HINT = R"(
    {
        "schema-version": 1,
        "template": {
            "category-layout": "grid",
            "card-size": "large",
            "card-layout": "horizontal"
        },
        "components": {
            "title": "title"
        }
    }
)";
} // anonymous namespace


std::string const Query::NO_RESULTS_HINT = _("No applications available. Install new applications with the Libertine Manager.");
std::string const Query::ALL_RESULTS_FILTERED_HINT = _("All applications hidden. Reset filters or check the Hidden Desktop Apps department.");


Query::
Query(usc::CannedQuery const&    query,
      usc::SearchMetadata const& metadata,
      Libertine::Factory const&  libertine_factory,
      std::shared_ptr<HiddenApps> hidden,
      std::shared_ptr<Blacklist> blacklist)
  : usc::SearchQueryBase(query, metadata)
  , libertine_(libertine_factory())
  , hidden_(hidden)
  , blacklist_(blacklist)
{
}


void Query::
cancelled()
{
}


QStringList Query::
make_filters(usc::SearchReplyProxy const& reply) const
{
  auto filter_state = query().filter_state();
  QStringList excludes_by_filter;
  std::list<usc::FilterBase::SCPtr> app_filters;

  //make exclude scope filter for apps
  for (auto const& container: libertine_->get_container_list())
  {
    usc::OptionSelectorFilter::SPtr filter{usc::OptionSelectorFilter::create(container->id(),
                                                                             EXCLUDED_APPS_FILTER_TITLE + container->name(),
                                                                             true)};
    // filter apps from blacklist
    for (auto const& app: container->app_launchers())
    {
      auto app_info = parse_app_info(app.uri());

      if (hidden_->app_is_hidden(app_info.key))
      {
        continue;
      }

      if (!blacklist_->app_is_blacklisted(app_info.app_id, container->id()))
      {
        filter->add_option(app_info.key.toStdString(), app.name());
      }
    }

    // get apps manually filtered by user
    if (filter->has_active_option(filter_state))
    {
      auto filteredApps = filter->active_options(filter_state);
      for (auto const &app: filteredApps)
      {
        excludes_by_filter.append(QString::fromStdString(app->id()));
      }
    }

    if (!filter->options().empty())
    {
      app_filters.push_back(filter);
    }
  }

  if (!app_filters.empty())
  {
    reply->push(app_filters, filter_state);
  }

  return excludes_by_filter;
}


void Query::
show_hint(usc::SearchReplyProxy const& reply,
          std::string           const& reason) const
{
  auto hint_category = reply->register_category("hint", "", "", usc::CategoryRenderer(CATEGORY_HINT));
  usc::CategorisedResult res(hint_category);
  res.set_uri(usc::CannedQuery(query()).to_uri());
  res.set_title(reason);
  reply->push(res);
}


void Query::
run(usc::SearchReplyProxy const& reply)
{
  if (!hidden_->empty())
  {
    register_departments(reply);
  }

  // only provide filters in root department
  QStringList excludes_by_filter;
  if (query().department_id().empty())
  {
    excludes_by_filter = make_filters(reply);
  }

  QRegExp search_query(QString::fromStdString(query().query_string()), Qt::CaseInsensitive);
  bool has_no_apps = true,
       all_filtered = true;

  for (auto const& container: libertine_->get_container_list())
  {
    auto category = reply->register_category(container->id(),
                                             container->name(),
                                             "Application",
                                             usc::CategoryRenderer(CATEGORY_APPS_DISPLAY));

    for (auto const& app: container->app_launchers())
    {
      has_no_apps = false;
      if (!(search_query.isEmpty() || QString::fromStdString(app.name()).contains(search_query)))
      {
        continue;
      }

      auto app_info = parse_app_info(app.uri());

      if (blacklist_->app_is_blacklisted(app_info.app_id, container->id()))
      {
        continue;
      }

      if (excludes_by_filter.contains(app_info.key))
      {
        continue;
      }

      // ignore hidden apps in root department
      if (query().department_id().empty() || query().department_id() == ROOT_DEPT_ID)
      {
        if (hidden_->app_is_hidden(app_info.key))
        {
          continue;
        }
      }
      else if (!hidden_->app_is_hidden(app_info.key))
      {
        continue;
      }

      usc::CategorisedResult result(category);
      result.set_title(app.name());
      result.set_art(app.icon());
      result.set_uri(app.uri());
      result[DESCRIPTION_FIELD] = app.description();
      result[APP_ID_FIELD] = app_info.key.toStdString();
      result[DEPARTMENT_ID_FIELD] = (query().department_id().empty() || query().department_id() == ROOT_DEPT_ID) ? ROOT_DEPT_ID : HIDDEN_DEPT_ID;

      if (!reply->push(result))
      {
        return;
      }

      all_filtered = false;
    }
  }

  if (has_no_apps)
  {
    show_hint(reply, NO_RESULTS_HINT);
  }
  else if (all_filtered)
  {
    show_hint(reply, ALL_RESULTS_FILTERED_HINT);
  }
}
