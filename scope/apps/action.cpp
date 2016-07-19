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

#include "scope/apps/action.h"
#include "scope/apps/config.h"
#include "scope/apps/hidden_apps.h"
#include <unity/scopes/ActivationResponse.h>
#include <unity/scopes/CannedQuery.h>
#include <url-dispatcher.h>
#include <QString>
#include <QFile>
#include <QTextStream>

namespace usc = unity::scopes;


Action::
Action(usc::Result const& result,
       usc::ActionMetadata const& metadata,
       std::string const& action_id, 
       std::shared_ptr<HiddenApps> hidden)
    : usc::ActivationQueryBase(result, metadata),
      action_id_(action_id),
      hidden_(hidden)
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
    hidden_->add(QString::fromStdString(result()["app_id"].get_string()));

    usc::CannedQuery cq(SCOPE_PKG);
    return usc::ActivationResponse(cq);
  }
  else if (action_id_ == "show")
  {
    hidden_->remove(QString::fromStdString(result()["app_id"].get_string()));

    usc::CannedQuery cq(SCOPE_PKG);
    return usc::ActivationResponse(cq);
  }
  return usc::ActivationResponse(usc::ActivationResponse::Status::NotHandled);
}
