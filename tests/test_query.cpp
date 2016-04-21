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
      "desktop_file_name": "/link/to/libreoffice.desktop"
    }, {
      "name": "Linux",
      "no_display": true,
      "desktop_file_name": "/link/to/linux.desktop"
    }, {
      "name": "Library",
      "no_display": false,
      "desktop_file_name": "/link/to/library.desktop"
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


MATCHER_P3(ResultPropertiesMatch, title, art, uri, "")
{
  return arg.contains("title") && arg.contains("art") && arg.contains("uri") &&
      arg["title"] == unity::scopes::Variant(title) &&
      arg["art"] == unity::scopes::Variant(art) &&
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
    , category(std::make_shared<FakeCategory>("fake", "fake-container", "Application", unity::scopes::CategoryRenderer()))
  {
  }

  void expect_registry()
  {
    EXPECT_CALL(reply, register_category("fake", "fake-container", "Application", testing::_)).WillOnce(testing::Return(category));
  }

  void expect_push(std::string title, std::string art, std::string appId, bool success = true)
  {
    EXPECT_CALL(reply, push(testing::Matcher<unity::scopes::CategorisedResult const&>(ResultPropertiesMatch(title, art, appId)))).WillOnce(testing::Return(success));
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
  expect_push("LibreOffice", "", "appid://fake/libreoffice/0.0");
  expect_push("Library", "", "appid://fake/library/0.0");

  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


TEST_F(TestQueryFixture, returnsRegularExpressionFilteredListOfApps)
{
  expect_registry();
  expect_push("LibreOffice", "", "appid://fake/libreoffice/0.0");
  canned_query.set_query_string("li.*office");

  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


TEST_F(TestQueryFixture, haltsFurtherPushesAfterFailedPush)
{
  expect_registry();
  expect_push("LibreOffice", "", "appid://fake/libreoffice/0.0", false);

  Query query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.run(proxy);
}


// Query class with faked out Settings
class QueryWithFakeSettings : public Query
{
public:
  QueryWithFakeSettings(unity::scopes::CannedQuery const& query,
        unity::scopes::SearchMetadata const&  metadata,
        Libertine::Factory const&   libertine_factory)
  : Query(query, metadata, libertine_factory)
    , settings_()
  {
  }

  unity::scopes::VariantMap settings() const override
  {
    return settings_;
  }

  unity::scopes::VariantMap settings_;
};


TEST_F(TestQueryFixture, ignoresAnyBlacklistedApps)
{
  expect_registry();
  expect_push("Library", "", "appid://fake/library/0.0");

  QueryWithFakeSettings query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.settings_["blacklist"] = "LibreOffice;Linux";
  query.run(proxy);
}


TEST_F(TestQueryFixture, stripsQuotationMarksFromBlacklist)
{
  expect_registry();
  expect_push("Library", "", "appid://fake/library/0.0");

  QueryWithFakeSettings query(canned_query, metadata, []() {
    return FakeLibertine::make_fake(LIBERTINE_OUTPUT_WITH_APPS);
  });
  query.settings_["blacklist"] = "\"LibreOffice\"";
  query.run(proxy);
}

} // anonymous namespace
