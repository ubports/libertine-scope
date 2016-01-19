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

#include "libertine-scope/applauncher.h"
#include "libertine-scope/container.h"
#include "libertine-scope/libertine.h"
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

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
                "fill-mode": "fit"
            }
        }
    }
)";


/**
 * Generates a ubuntu-application-launcher URI for a contained desktop file.
 */
static std::string
app_uri(Container const& container, AppLauncher const& app)
{
  return "appid://" + container.id() + "/" + app.id() + "/0.0";
}

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


void Query::
run(usc::SearchReplyProxy const& reply)
{
  usc::CannedQuery const& query(usc::SearchQueryBase::query());
  std::string query_string = query.query_string();

  Libertine::UPtr libertine = libertine_factory_();
  for (auto const& container: libertine->get_container_list())
  {
    auto category = reply->register_category(container->id(),
                                             container->name(),
                                             "Application",
                                             usc::CategoryRenderer(CATEGORY_APPS_DISPLAY));

    for (auto const& app: container->app_launchers())
    {
      if (app.no_display())
        continue;

      usc::CategorisedResult result(category);
      result.set_title(app.name());
      result.set_art(app.icon());
      result.set_uri(app_uri(*container, app));
      if (!reply->push(result))
      {
        break;
      }
    }
  }
}

