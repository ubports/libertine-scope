/*
 * Copyright (C) 2016 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * * Authored by:
 *	Kyle Nitzsche <kyle.nitzsche@canonical.com>
 */

#include "libertine-scope/action.h"
#include "libertine-scope/config.h"
#include <unity/scopes/ActivationResponse.h>
#include <unity/scopes/CannedQuery.h>
#include <url-dispatcher.h>
#include <QString>
#include <QFile>
#include <QTextStream>

namespace usc = unity::scopes;

namespace
{

static void
write_apps_to_hidden(QFile &hidden_f, QStringList const& hidden)
{ 
  hidden_f.open(QIODevice::WriteOnly | QIODevice::Text);	
  QTextStream out(&hidden_f);
  for (auto const& app : hidden)
  {
    out << app << "\n";
  }
  hidden_f.flush();
  hidden_f.close();
}

} //anonymous namespace


Action::
Action(usc::Result const& result,
       usc::ActionMetadata const& metadata,
       std::string const& action_id, 
       std::string const& cache_dir)
    : usc::ActivationQueryBase(result, metadata),
      action_id_(action_id),
      cache_dir_(cache_dir)
{
}

usc::ActivationResponse
Action::activate()
{
  if (action_id_ == "open")
  {
    url_dispatch_send(result().uri().c_str() , NULL, NULL);
    return usc::ActivationResponse(usc::ActivationResponse::Status::NotHandled);
  }
  else if (action_id_ == "hide")
  {
    QFile file(QString("%1/%2").arg(QString::fromStdString(cache_dir_),"hidden"));
    file.open(QIODevice::Append | QIODevice::Text);	
    QTextStream out(&file);
    out << QString::fromStdString(result()["app_id"].get_string());
    endl(out);
    file.close();
    usc::CannedQuery cq(SCOPE_PKG + "_" + SCOPE_APP);
    return usc::ActivationResponse(cq);
  }
  else if (action_id_ == "show")
  {
    QFile hidden_f(QString("%1/%2").arg(QString::fromStdString(cache_dir_), QString::fromStdString("hidden")));
    if (hidden_f.exists()) {
      QStringList hidden;
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
      QString app_id = QString::fromStdString(result()["app_id"].get_string());
      if (hidden.contains(app_id))
      {
        hidden.removeAll(app_id);
        write_apps_to_hidden(hidden_f, hidden);
      }
    }
    usc::CannedQuery cq(SCOPE_PKG + "_" + SCOPE_APP);
    return usc::ActivationResponse(cq);
  }
  return usc::ActivationResponse(usc::ActivationResponse::Status::NotHandled);
}
