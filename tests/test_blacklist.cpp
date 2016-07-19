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
#include "scope/apps/blacklist.h"
#include <gtest/gtest.h>
#include <QDir>
#include <QTemporaryDir>


namespace
{
class TestBlacklistFixture : public ::testing::Test
{
public:
  TestBlacklistFixture()
    : blacklist(QString("%1/data").arg(QDir::currentPath()).toStdString())
  {
  }

protected:
  Blacklist blacklist;
};

TEST_F(TestBlacklistFixture, ReturnsFalseWhenFileDoesNotExist)
{
  EXPECT_FALSE(Blacklist(QTemporaryDir().path().toStdString()).app_is_blacklisted("app1", "container1"));
}

TEST_F(TestBlacklistFixture, ReturnsFalseWhenAppIsNotInTheList)
{
  EXPECT_FALSE(blacklist.app_is_blacklisted("app5", "container1"));
}

TEST_F(TestBlacklistFixture, ReturnsTrueWhenAppIsBlacklistedLocally)
{
  EXPECT_TRUE(blacklist.app_is_blacklisted("app1", "container1"));
}

TEST_F(TestBlacklistFixture, ReturnsTrueWhenAppIsBlacklistedGlobally)
{
  EXPECT_TRUE(blacklist.app_is_blacklisted("app2", "container2"));
}

TEST_F(TestBlacklistFixture, ReturnsFalseWhenAppIsWhitelistedLocally)
{
  EXPECT_FALSE(blacklist.app_is_blacklisted("app3", "container1"));
}

TEST_F(TestBlacklistFixture, ReturnsFalseWhenAppIsWhitelistedGlobally)
{
  EXPECT_FALSE(blacklist.app_is_blacklisted("app4", "container2"));
}
}
