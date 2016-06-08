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

#include "libertine-scope/query.h"
#include "tests/fake_libertine.h"
#include <unity/scopes/SearchMetadata.h>
#include <unity/scopes/CannedQuery.h>
#include <unity/scopes/SearchReplyProxyFwd.h>
#include <unity/scopes/testing/MockSearchReply.h>
#include <unity/scopes/Category.h>
#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace
{
const std::string LIBERTINE_OUTPUT_WITH_APPS = R"(
  {
    "name": "fake-container",
    "app_launchers": [{
      "name": "LibreOffice",
      "no_display": false,
      "uri": "appid://fake-container/libreoffice/0.0",
      "icons": ["file:///lo.png"],
      "description": "libreoffice!"
    }, {
      "name": "Linux",
      "no_display": true,
      "uri": "appid://fake-container/linux/0.0",
      "icons": ["file:///nix.png"],
      "description": "linux!"
    }, {
      "name": "Library",
      "no_display": false,
      "uri": "appid://fake-container/library/0.0",
      "icons": ["file:///lib.png"],
      "description": "library!"
    }]
  }
)";


class FakeCategory : public unity::scopes::Category
{
public:
    FakeCategory(std::string const& id, std::string const& title,
                 std::string const& icon, unity::scopes::CategoryRenderer const& renderer) :
       unity::scopes::Category(id, title, icon, renderer)
    {
    }

};


MATCHER_P4(ResultPropertiesMatch, title, art, description, uri, "")
{
  return arg.contains("title") && arg.contains("art") && arg.contains("description") && arg.contains("uri") &&
      arg["title"] == unity::scopes::Variant(title) &&
      arg["art"] == unity::scopes::Variant(art) &&
      arg["description"] == unity::scopes::Variant(description) &&
      arg["uri"] == unity::scopes::Variant(uri);
}


class TestQueryFixture : public ::testing::Test
{
public:
  TestQueryFixture()
    : metadata("en_US", "phone")
    , canned_query("libertine-scope")
    , reply()
    , proxy(&reply, [](unity::scopes::SearchReply*) {})
    , category(std::make_shared<FakeCategory>("fakeId", "fake-container", "Application", unity::scopes::CategoryRenderer()))
  {
  }

  void expect_registry()
  {
    EXPECT_CALL(reply, register_category("fakeId", "fake-container", "Application", testing::_)).WillOnce(testing::Return(category));
  }

  void expect_push(std::string title, std::string art, std::string description, std::string appId, bool success = true)
  {
    EXPECT_CALL(reply, push(testing::Matcher<unity::scopes::CategorisedResult const&>(ResultPropertiesMatch(title, art, description, appId)))).WillOnce(testing::Return(success));
  }

  void expect_push_libreoffice(bool success = true)
  {
    expect_push("LibreOffice", "file:///lo.png", "libreoffice!", "appid://fake-container/libreoffice/0.0", success);
  }

  void expect_push_library(bool success = true)
  {
    expect_push("Library", "file:///lib.png", "library!", "appid://fake-container/library/0.0", success);
  }

  void expect_push_linux(bool success = true)
  {
    expect_push("Linux", "file:///nix.png", "linux!", "appid://fake-container/linux/0.0", success);
  }

  unity::scopes::SearchMetadata metadata;
  unity::scopes::CannedQuery canned_query;
  testing::NiceMock<unity::scopes::testing::MockSearchReply> reply;
  unity::scopes::SearchReplyProxy proxy;
  std::shared_ptr<FakeCategory> category;
};


TEST_F(TestQueryFixture, returnsAllDisplayableAppsWithoutFilters)
{
  expect_registry();
  expect_push_libreoffice();
  expect_push_linux();
  expect_push_library();
  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


TEST_F(TestQueryFixture, returnsRegularExpressionFilteredListOfApps)
{
  expect_registry();
  expect_push_libreoffice();
  canned_query.set_query_string("li.*office");

  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


TEST_F(TestQueryFixture, haltsFurtherPushesAfterFailedPush)
{
  expect_registry();
  expect_push_libreoffice(false);

  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


TEST_F(TestQueryFixture, ignoresAnyBlacklistedApps)
{
  expect_registry();
  expect_push_linux();
  expect_push_library();
  QStringList blacklist = {"fake-container/libreoffice"};
  QStringList whitelist = {};
  std::tuple<QStringList,QStringList> lists(blacklist, whitelist);
  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  }, "/tmp", lists);
  query.run(proxy);
}


TEST_F(TestQueryFixture, displayWhitelistedApps)
{
  expect_registry();
  expect_push_linux();
  expect_push_library();
  QStringList blacklist = {"all/libreoffice"};
  QStringList whitelist = {"fakeId/liberoffice"};
  std::tuple<QStringList,QStringList> lists(blacklist, whitelist);
  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  }, "/tmp", lists);
  query.run(proxy);
}

} // anonymous namespace
