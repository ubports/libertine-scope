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

#ifndef SCOPE_ACTION_H_
#define SCOPE_ACTION_H_

#include "libertine-scope/scope.h"
#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/ActivationQueryBase.h>
#include <unity/scopes/ActivationResponse.h>
#include <unity/scopes/Result.h>

class Action : public unity::scopes::ActivationQueryBase {
  public:
    Action(unity::scopes::Result const&         result,
           unity::scopes::ActionMetadata const& metadata,
           std::string const&                   action_id, 
           std::string const&                   cache_dir);

    virtual ~Action() = default;
    virtual unity::scopes::ActivationResponse activate() override;

  private:
    std::string action_id_;
    std::string cache_dir_;
};
#endif
