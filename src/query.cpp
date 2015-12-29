/*
 * Copyright 2015 Canonical Ltd.
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
#include <query.h>
#include <localization.h>

#include "applauncher.h"
#include "container.h"
#include "libertine.h"
#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <iostream>

namespace usc = unity::scopes;

using namespace std;


Query::
Query(usc::CannedQuery const& query,
      usc::SearchMetadata const& metadata)
: usc::SearchQueryBase(query, metadata)
{
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << " begins\n";
  std::cerr << "==smw>   query_string='" << query.query_string() << "'\n";
  std::cerr << "==smw>   to_uri='" << query.to_uri() << "'\n";
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << " ends\n";
}


void Query::
cancelled()
{
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
}


void Query::
run(usc::SearchReplyProxy const& reply)
{
  // Start by getting information about the query
  usc::CannedQuery const& query(usc::SearchQueryBase::query());

  // Get the query string
  string query_string = query.query_string();


  Libertine libertine;
  for (auto const& container: libertine.get_container_list())
  {
    auto category = reply->register_category(container->id(), container->name(), "icon");
    for (auto const& app: container->app_launchers())
    {
      if (app.no_display())
        continue;

      usc::CategorisedResult result(category);
      result.set_title(app.name());
      result.set_art("file://" + app.icon());
      result.set_uri("application://" + app.desktop_file());
      if (!reply->push(result))
      {
        break;
      }
    }
  }
}

