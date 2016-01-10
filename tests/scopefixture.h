/*
 * Copyright 2016 Canonical Ltd.
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
#ifndef LIBERTINE_SCOPE_SCOPEFIXTURE_H_
#define LIBERTINE_SCOPE_SCOPEFIXTURE_H_

#include <iostream>
#include "src/scope.h"
//#include <unity/scopes/testing/TypedScopeFixture.h>
#include "tests/TypedScopeFixture.h"


namespace unity
{
namespace scopes
{
namespace testing
{

/**
 * Specialize the ScopeTraist for our scope.
 */
template<>
  struct ScopeTraits<::Scope>
  {
    static char const*
    name()
    {
      std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "()\n";
      return "LibertineScope";
    }

    static std::shared_ptr<::Scope>
    construct()
    {
      std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "()\n";
      return std::make_shared<::Scope>();
    }
  };

} // namespace testing
} // namespace scopes
} // namespace unity

typedef unity::scopes::testing::TypedScopeFixture<Scope> ScopeFixture;

#endif /* LIBERTINE_SCOPE_SCOPEFIXTURE_H_ */
