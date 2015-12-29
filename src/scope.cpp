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
#include <scope.h>

#include <fstream>
#include <iostream>
#include <localization.h>
#include <query.h>
#include <sstream>


namespace usc = unity::scopes;
using namespace std;

void Scope::
start(string const&)
{
  setlocale(LC_ALL, "");
  string translation_directory = ScopeBase::scope_directory()
          + "/../share/locale/";
  bindtextdomain(GETTEXT_PACKAGE, translation_directory.c_str());
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
}


void Scope::
stop()
{
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
}


usc::SearchQueryBase::UPtr Scope::
search(usc::CannedQuery const&    query,
       usc::SearchMetadata const& metadata)
{
  // Boilerplate construction of Query
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "() metadata.is_aggregated()=" << metadata.is_aggregated() <<"\n";
  return usc::SearchQueryBase::UPtr(new Query(query, metadata));
}


usc::PreviewQueryBase::UPtr Scope::
preview(usc::Result const&         result,
        usc::ActionMetadata const& metadata)
{
  // Boilerplate construction of Preview
  //return usc::PreviewQueryBase::UPtr(new Preview(result, metadata));
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
}


#define EXPORT __attribute__ ((visibility ("default")))

// These functions define the entry points for the scope plugin
extern "C" {

EXPORT unity::scopes::ScopeBase*
// cppcheck-suppress unusedFunction
UNITY_SCOPE_CREATE_FUNCTION()
{
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
  return new Scope();
}


EXPORT void
// cppcheck-suppress unusedFunction
UNITY_SCOPE_DESTROY_FUNCTION(unity::scopes::ScopeBase* scope_base)
{
  std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
  delete scope_base;
}

}

