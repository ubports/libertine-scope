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
#include "libertine-scope/scope.h"

#include "libertine-scope/preview.h"
#include "libertine-scope/query.h"
#include "libertine-scope/action.h"
#include <localization.h>
#include <sstream>
#include <url-dispatcher.h>
#include <QFile>
#include <QTextStream>
#include <QString>


namespace usc = unity::scopes;

namespace
{

static bool
is_whitelist(QString const& line)
{
  if (line.startsWith("whitelist"))
  {
    if (line.split("/").size() < 3)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return false;
  } 
}

static QString
get_whitelist_key(QString const& line)
{
  QStringList parts = line.split("/");
  return parts[1] + "/" + parts[2].trimmed();
}

static std::tuple<QStringList,QStringList>
get_bwlists(std::string scope_dir)
{
  //Get blacklisted and whitelisted apps from "blacklist" file in scope dir
  QStringList blacklist;
  QStringList whitelist;
  QFile blacklist_f(QString("%1/%2").arg(QString::fromStdString(scope_dir), QString::fromStdString("blacklist")));
  if (blacklist_f.exists()) {
    if (blacklist_f.open(QIODevice::ReadOnly | QIODevice::Text)){
      QTextStream in(&blacklist_f);
      while (!in.atEnd()) {
        QString line(in.readLine());
        if (!line.startsWith("#") && !line.startsWith("whitelist"))
        {
          blacklist.append(line.trimmed());
        }
        if (is_whitelist(line))
        {
          whitelist.append(get_whitelist_key(line));
        }
      }
      blacklist_f.close();
    }
  }
  return std::tie(blacklist,whitelist);
}

} // anonymous namespace


Scope::
Scope(Libertine::Factory const& libertine_factory)
: libertine_factory_(libertine_factory)
{
}


void Scope::
start(std::string const&)
{
  setlocale(LC_ALL, "");
  std::string translation_directory = ScopeBase::scope_directory()
                                    + "/../share/locale/";
  bindtextdomain(GETTEXT_PACKAGE, translation_directory.c_str());
}


void Scope::
stop()
{
}


usc::SearchQueryBase::UPtr Scope::
search(usc::CannedQuery const&    query,
       usc::SearchMetadata const& metadata)
{
  return usc::SearchQueryBase::UPtr(new Query(query,
                                              metadata,
                                              libertine_factory_,
                                              cache_directory(),
                                              get_bwlists(scope_directory())));
}


usc::PreviewQueryBase::UPtr Scope::
preview(usc::Result const&         result,
        usc::ActionMetadata const& metadata)
{
  return usc::PreviewQueryBase::UPtr(new Preview(result, metadata));
}


usc::ActivationQueryBase::UPtr Scope::
perform_action(usc::Result const&         result,
               usc::ActionMetadata const& metadata,
               std::string const&         /* widget_id */,
               std::string const&         action_id)
{
  return usc::ActivationQueryBase::UPtr(new Action(result, metadata, action_id, cache_directory()));
}


#define EXPORT __attribute__((visibility ("default")))

// These functions define the entry points for the scope plugin
extern "C" {

EXPORT unity::scopes::ScopeBase*
// cppcheck-suppress unusedFunction
UNITY_SCOPE_CREATE_FUNCTION()
{
  return new Scope();
}


EXPORT void
// cppcheck-suppress unusedFunction
UNITY_SCOPE_DESTROY_FUNCTION(unity::scopes::ScopeBase* scope_base)
{
  delete scope_base;
}

}

