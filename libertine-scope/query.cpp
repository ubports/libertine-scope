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
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>
#include <QString>
#include <QStringList>
#include <QRegExp>


namespace usc = unity::scopes;


namespace
{

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
      Libertine::Factory const&  libertine_factory)
: usc::SearchQueryBase(query, metadata)
, libertine_factory_(libertine_factory)
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


QStringList
Query::blacklist() const
{
  QStringList blacklistedApps;
  auto blacklist = settings()["blacklist"];
  if (!blacklist.is_null()) {
    blacklistedApps = QString::fromStdString(blacklist.get_string())
        .remove("\"")
        .split(";", QString::SkipEmptyParts);
  }
  return blacklistedApps;
}


void Query::
run(usc::SearchReplyProxy const& reply)
{
  auto blacklistedApps = blacklist();
  QRegExp re(QString::fromStdString(query().query_string()), Qt::CaseInsensitive);
  Libertine::UPtr libertine = libertine_factory_();

  for (auto const& container: libertine->get_container_list())
  {
    auto category = reply->register_category(container->id(),
                                             container->name(),
                                             "Application",
                                             usc::CategoryRenderer(CATEGORY_APPS_DISPLAY));
    for (auto const& app: container->app_launchers())
    {
      if (!(re.isEmpty() || QString::fromStdString(app.name()).contains(re))
          || blacklistedApps.contains(QString::fromStdString(app.name())))
      {
        continue;
      }

      usc::CategorisedResult result(category);
      result.set_title(app.name());
      result.set_art(app.icon());
      result.set_uri(app.uri());
      result["description"] = app.description();
      if (!reply->push(result))
      {
        break;
      }
    }
  }
}

