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
#include "scope/apps/action.h"
#include "tests/mock_hidden_apps.h"

#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/testing/Result.h>
#include <gmock/gmock.h>


namespace
{
TEST(ActionTest, OpenSendsDispatchURI)
{
  unity::scopes::testing::Result result;
  result.set_uri("app://this/is/my/app");
  unity::scopes::ActionMetadata metadata("en_US", "phone");
  auto hidden = std::make_shared<testing::NiceMock<MockHiddenApps> >();
  unity::scopes::FilterState filter_state;

  std::string uri;

  Action action(result, metadata, "open", [&](std::string const& app_uri) {
    uri = app_uri;
  }, hidden, filter_state);

  auto response = action.activate();
  EXPECT_EQ(unity::scopes::ActivationResponse::Status::NotHandled, response.status());
  EXPECT_EQ(result.uri(), uri);
}


TEST(ActionTest, ShowRemovesAppFromHidden)
{
  unity::scopes::testing::Result result;
  unity::scopes::ActionMetadata metadata("en_US", "phone");
  unity::scopes::FilterState filter_state;
  auto hidden = std::make_shared<testing::NiceMock<MockHiddenApps> >();

  QString app_id = "something.desktop";
  result["app_id"] = app_id.toStdString();
  EXPECT_CALL(*hidden, remove(app_id));

  Action action(result, metadata, "show", Action::OpenUriAction{}, hidden, filter_state);

  auto response = action.activate();
  EXPECT_EQ(filter_state.serialize(), response.query().filter_state().serialize());
  EXPECT_EQ(unity::scopes::ActivationResponse::Status::PerformQuery, response.status());
}


TEST(ActionTest, HideAddsAppToHidden)
{
  unity::scopes::testing::Result result;
  unity::scopes::ActionMetadata metadata("en_US", "phone");
  unity::scopes::FilterState filter_state;
  auto hidden = std::make_shared<testing::NiceMock<MockHiddenApps> >();

  QString app_id = "something.desktop";
  result["app_id"] = app_id.toStdString();
  EXPECT_CALL(*hidden, add(app_id));

  Action action(result, metadata, "hide", Action::OpenUriAction{}, hidden, filter_state);

  auto response = action.activate();
  EXPECT_EQ(filter_state.serialize(), response.query().filter_state().serialize());
  EXPECT_EQ(unity::scopes::ActivationResponse::Status::PerformQuery, response.status());
}
}
