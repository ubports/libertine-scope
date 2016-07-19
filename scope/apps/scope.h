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
#ifndef LIBERTINE_SCOPE_SCOPE_H_
#define LIBERTINE_SCOPE_SCOPE_H_

#include "scope/apps/libertine.h"
#include <unity/scopes/PreviewQueryBase.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>
#include <unity/scopes/ScopeBase.h>


/**
 * Controller for the entire scope.
 *
 * An instance of this class gets instantiated when the scope plugin gets loaded
 * by the thing that loads scope plugins.
 */
class Scope
: public unity::scopes::ScopeBase
{
public:
  /**
   * Constructs a Scope.
   *
   * @param[in] libertine_factory Creates a Libertine Proxy instance.  The
   *                              default it to proxy the Libertine CLI tools.
   */
  Scope(Libertine::Factory const& libertine_factory = Libertine::from_libertine_cli);

  /**
   * Initializes the scope instance.
   */
  void
  start(std::string const&) override;

  /**
   * Tears down the scope instance.
   */
  void
  stop() override;

  /**
   * Called each time a new query is requested
   */
  unity::scopes::SearchQueryBase::UPtr
  search(unity::scopes::CannedQuery const&    query,
         unity::scopes::SearchMetadata const& metadata) override;

  /**
   * Gets an application preview.
   */
  unity::scopes::PreviewQueryBase::UPtr
  preview(unity::scopes::Result const&         result,
          unity::scopes::ActionMetadata const& metadata) override;

  /**
   * Performs an action in response to user interaction with the preview.
   */
  unity::scopes::ActivationQueryBase::UPtr
  perform_action(unity::scopes::Result const&         result,
                 unity::scopes::ActionMetadata const& metadata,
                 std::string const&                   widget_id,
                 std::string const&                   action_id) override;

private:
  Libertine::Factory libertine_factory_;
};

#endif // LIBERTINE_SCOPE_SCOPE_H_

