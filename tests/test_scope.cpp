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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include "tests/scopefixture.h"
#include <unity/scopes/CannedQuery.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/SearchMetadata.h>
#include <unity/scopes/SearchReply.h>
#include <unity/scopes/SearchReplyProxyFwd.h>
#include <unity/scopes/testing/Category.h>
#include <unity/scopes/testing/MockSearchReply.h>


TEST_F(ScopeFixture, testConstruction)
{
}


TEST_F(ScopeFixture, surfacing_query)
{
  using namespace testing;
  using namespace unity::scopes;
  using namespace unity::scopes::testing;

  const unity::scopes::CategoryRenderer renderer;

  CannedQuery    query("libertine-scope", "", "");
  SearchMetadata meta_data("en_EN", "phone");
  auto search_query = scope->search(query, meta_data);
  ASSERT_NE(nullptr, search_query);

  NiceMock<MockSearchReply> reply;
  EXPECT_CALL(reply, register_category(_, _, _, _))
    .Times(1)
    .WillOnce(Return(unity::scopes::Category::SCPtr(new unity::scopes::testing::Category("x", "y", "z", renderer))));
  EXPECT_CALL(reply, push(Matcher<CategorisedResult const&>(_))).Times(2).WillRepeatedly(Return(true));

  SearchReplyProxy search_reply_proxy(&reply, [](unity::scopes::SearchReply*) {});
  search_query->run(search_reply_proxy);
}

