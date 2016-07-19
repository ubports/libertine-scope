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

#include "scope/apps/preview.h"
#include <unity/scopes/Variant.h>
#include <gtest/gtest.h>
#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/testing/Result.h>
#include <unity/scopes/testing/MockPreviewReply.h>
#include <unity/scopes/PreviewWidget.h>
#include <memory>
#include <QDir>


TEST(TestPreview, pushesWidgetsWithAppInformation)
{
  unity::scopes::testing::Result result;
  result["department_id"] = "";
  unity::scopes::ActionMetadata metadata("en_US", "phone");

  std::unique_ptr<unity::scopes::PreviewWidgetList> list(new unity::scopes::PreviewWidgetList());
	testing::NiceMock<unity::scopes::testing::MockPreviewReply> reply;
  EXPECT_CALL(reply, push(testing::_)).WillOnce(testing::SaveArg<0>(list.get()));
  unity::scopes::PreviewReplyProxy proxy(&reply, [](unity::scopes::PreviewReply*) {});

	Preview preview(result, metadata);
	preview.run(proxy);

  ASSERT_NE(nullptr, list);
  ASSERT_EQ(3, list->size());

  auto header = list->front();
  EXPECT_EQ("hdr", header.id());
  EXPECT_EQ("header", header.widget_type());
  EXPECT_EQ("title", header.attribute_mappings()["title"]);
  EXPECT_EQ("art", header.attribute_mappings()["mascot"]);

  list->pop_front();
  auto desc = list->front();
  EXPECT_EQ("desc", desc.id());

  list->pop_front();
  auto buttons = list->front();
  EXPECT_EQ("buttons", buttons.id());
  EXPECT_EQ("actions", buttons.widget_type());

  auto buttons_actions = buttons.attribute_values()["actions"].get_array();
  ASSERT_EQ(2, buttons_actions.size());
  EXPECT_EQ("open", buttons_actions[0].get_dict()["id"].get_string());
  EXPECT_EQ("Open", buttons_actions[0].get_dict()["label"].get_string());
}
