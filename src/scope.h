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
#ifndef SCOPE_H_
#define SCOPE_H_

#include <unity/scopes/PreviewQueryBase.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>
#include <unity/scopes/ScopeBase.h>


/**
 */
class Scope
: public unity::scopes::ScopeBase
{
public:
  /**
   * Called once at startup.
   */
  void
  start(std::string const&) override;

  /**
   * Called at shutdown.
   */
  void
  stop() override;

  /**
   * Called each time a new preview is requested
   */
  unity::scopes::PreviewQueryBase::UPtr
  preview(unity::scopes::Result const&         result,
          unity::scopes::ActionMetadata const& metadata) override;

  /**
   * Called each time a new query is requested
   */
  unity::scopes::SearchQueryBase::UPtr
  search(unity::scopes::CannedQuery const&    query,
         unity::scopes::SearchMetadata const& metadata) override;

  unity::scopes::ActivationQueryBase::UPtr
  perform_action(unity::scopes::Result const&         result,
                 unity::scopes::ActionMetadata const& metadata,
                 std::string const&                   widget_id,
                 std::string const&                   action_id) override;
};

#endif // SCOPE_H_

