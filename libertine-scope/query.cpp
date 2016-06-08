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

#include "libertine-scope/query.h"
#include "libertine-scope/container.h"
#include "libertine-scope/config.h"
#include "localization.h"
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
#include <QDebug>
#include <QTextStream>

namespace usc = unity::scopes;

namespace
{

//returns map of apps with keys: container, app, name
static std::map<QString,QString>
app_keys(std::string const& uri)
{
  std::map<QString,QString> parts;
  QStringList uri_parts = QString::fromStdString(uri).split("/");
  if (uri_parts.size() < 4)
  {
    return parts;
  }
  parts["container"] = uri_parts[2];
  parts["name"] = uri_parts[3];
  parts["key"] = QString("%1/%2").arg(parts["container"],parts["name"]);
  return parts;
}

static QStringList
get_hidden(std::string const& cache_dir)
{
  QStringList hidden;
  QFile hidden_f(QString("%1/%2").arg(QString::fromStdString(cache_dir), QString::fromStdString("hidden")));
  if (hidden_f.exists())
  {
    if (hidden_f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream in(&hidden_f);
      while (!in.atEnd())
      {
        QString line(in.readLine());
        hidden.append(line.trimmed());
      }   
      hidden_f.close();
    }   
  }
  return hidden;
}

static void
make_departments(usc::SearchReplyProxy const& reply)
{
  usc::Department::SPtr root_dept; 
  usc::Department::SPtr hidden_apps_dept; 
  usc::CannedQuery myquery("libertine-scope.ubuntu");
  myquery.set_department_id(ROOT_DEPT_ID);
  root_dept = std::move(usc::Department::create("", myquery, _("X Apps")));
  myquery.set_department_id(HIDDEN_DEPT_ID);
  hidden_apps_dept = std::move(usc::Department::create("hidden", myquery, _("Hidden X Apps")));
  root_dept->add_subdepartment(hidden_apps_dept);
  reply->register_departments(root_dept);
}

static void
make_filters(usc::SearchReplyProxy const& reply,
             Libertine::UPtr              libertine,
             QStringList           const& hidden,
             QStringList           const& whitelist_,
             QStringList           const& blacklist_,
             QStringList                & excludes_by_filter,
             usc::FilterState      const& filter_state
            )
{
  std::map<std::string,unity::scopes::OptionSelectorFilter::SPtr> filters;
  //make exclude scope filter for apps
  for (auto const& container: libertine->get_container_list())
  {
    std::string filter_title = std::string(_("Exclude Apps")) + ": " + container->name();
    filters[container->id()] = unity::scopes::OptionSelectorFilter::create(container->id(), filter_title, true);
    unity::scopes::OptionSelectorFilter::SPtr filter = filters[container->id()];
    // add exclude filter checkable items for each app that pass blacklisting + whitelisting
    for (auto const& app: container->app_launchers())
    {
      auto excludes_map = app_keys(app.uri());
      //only exlcude through blacklisting if not explicitly whitelisted
      QString app_id = QString::fromStdString(app.uri()).split("/")[3];
      QString key = QString::fromStdString(container->id()) + "/" + app_id;
      if (hidden.contains(key))
      {
        continue;
      }
      if (whitelist_.contains(key))
      {
        filter->add_option(excludes_map["key"].toStdString(), app.name());
      }
      else if (!blacklist_.contains(excludes_map["key"]) && !blacklist_.contains("all/" + excludes_map["name"]))
      {
        filter->add_option(excludes_map["key"].toStdString(), app.name());
      }
    }
    //get exclude filter checked items
    if (filter->has_active_option(filter_state))
    {
      auto excludeTypes = filter->active_options(filter_state);
      for (auto const &opt: excludeTypes)
      {
        excludes_by_filter.append(QString::fromStdString(opt->id()));
      }
    }
  }
  //push filters
  std::list<unity::scopes::FilterBase::SCPtr> filters_v;
  for (const auto& filter: filters)
  {
    if (filter.second->options().size() > 0 )
    {
      filters_v.push_back(filter.second);
    }
  }
  reply->push(filters_v, filter_state); 
}

/**
 * A custom rendering layout brazenly stolen from the click scope, so they look
 * sorta similar.  At least until they change theirs.
 */
std::string const CATEGORY_APPS_DISPLAY = R"(
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

} // anonymous namespace


Query::
Query(usc::CannedQuery const&    query,
      usc::SearchMetadata const& metadata,
      Libertine::Factory const&  libertine_factory,
      std::string const& cache_dir,
      std::tuple<QStringList,QStringList> const& blackwhiteLists
      )
: usc::SearchQueryBase(query, metadata)
, libertine_factory_(libertine_factory)
, cache_dir_(cache_dir)
, blacklist_(std::get<0>(blackwhiteLists))
, whitelist_(std::get<1>(blackwhiteLists))
{
}


void Query::
cancelled()
{
}
 

unity::scopes::VariantMap
Query::settings() const
{
  return SearchQueryBase::settings();
}


void Query::
run(usc::SearchReplyProxy const& reply)
{
  auto hidden = get_hidden(cache_dir_);
  // make scope departments
  if (hidden.size() > 0 )
  { 
    make_departments(reply);
  }

  std::pair<QStringList,QStringList> bwlists;
  std::map<QString,QString> excludes_map;
  QStringList excludes_by_filter;
  QRegExp re(QString::fromStdString(query().query_string()), Qt::CaseInsensitive);
  Libertine::UPtr libertine = libertine_factory_();

  //only provide filters in root department: X Apps
  if (query().department_id().empty())
  { 
    make_filters(reply, libertine_factory_(), hidden, whitelist_, blacklist_, excludes_by_filter, query().filter_state());
  }

  //make and push results
  for (auto const& container: libertine->get_container_list())
  {
    auto category = reply->register_category(container->id(),
                                             container->name(),
                                             "Application",
                                             usc::CategoryRenderer(CATEGORY_APPS_DISPLAY));
    bool breaking = false;
    for (auto const& app: container->app_launchers())
    {
      //search
      if (!(re.isEmpty() || QString::fromStdString(app.name()).contains(re)))
        continue;

      excludes_map = app_keys(app.uri());
      //only check for blacklisted if not whitelisted
      QString app_id;
      if (QString::fromStdString(app.uri()).split("/").size() >= 4)
        app_id = QString::fromStdString(app.uri()).split("/")[3];
      QString key = QString::fromStdString(container->id()) + "/" + app_id;
      //don't display if blacklisted
      if (!whitelist_.contains(key))
      {
        if (blacklist_.contains(excludes_map["key"]) || blacklist_.contains("all/" + excludes_map["name"]))
        {
          continue;
        }
      }
      //don't display apps excluded by filter
      if (excludes_by_filter.contains(excludes_map["key"]))
      {
        continue;
      }

      //if root department, don't display if it's a hidden app
      if (query().department_id().empty() || query().department_id() == ROOT_DEPT_ID)
      {
        if (hidden.contains(key))
        {
          continue;
        }
      }
      else
      {
        //don't display non hidden apps in hidden dept
        if (!hidden.contains(key))
        {
          continue;
        }
      }
      usc::CategorisedResult result(category);
      result.set_title(app.name());
      result.set_art(app.icon());
      result.set_uri(app.uri());
      result["description"] = app.description();
      result["app_id"] = key.toStdString();
      //add department id so we know which buttons to display on preview
      if (query().department_id().empty() || query().department_id() == ROOT_DEPT_ID)
      {
        result["department_id"] = ROOT_DEPT_ID;
      }
      else
      {
        result["department_id"] = "hidden";
      }
      if (!reply->push(result))
      {
        breaking = true;
        break;
      }
    }
    if (breaking)
    {
      break;
    }
  }
}
