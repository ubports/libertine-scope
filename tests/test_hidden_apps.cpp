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
#include "scope/apps/hidden_apps.h"
#include <gtest/gtest.h>
#include <QDir>
#include <QTemporaryDir>

namespace
{
class TestHiddenAppsFixture : public ::testing::Test
{
public:
  TestHiddenAppsFixture()
    : hidden(QString("%1/data").arg(QDir::currentPath()).toStdString())
  {
  }

  virtual void SetUp()
  {
    QFile file(QString("%1/data/hidden").arg(QDir::currentPath()));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      original_file_contents = file.readAll();
    }
  }

  virtual void TearDown()
  {
    QFile file(QString("%1/data/hidden").arg(QDir::currentPath()));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      file.write(original_file_contents);
    }
  }

protected:
  HiddenApps hidden;
  QByteArray original_file_contents;
};


TEST_F(TestHiddenAppsFixture, HiddenReturnsFalseWhenFileDoesNotExist)
{
  EXPECT_FALSE(HiddenApps(QTemporaryDir().path().toStdString()).app_is_hidden("container1/app1"));
}

TEST_F(TestHiddenAppsFixture, HiddenReturnsFalseWhenAppNotListed)
{
  EXPECT_FALSE(hidden.app_is_hidden("container2/app1"));
}

TEST_F(TestHiddenAppsFixture, HiddenReturnsFalseWhenAppIsListed)
{
  EXPECT_TRUE(hidden.app_is_hidden("container1/app1"));
}

TEST_F(TestHiddenAppsFixture, AddAppendsAppToFile)
{
  ASSERT_FALSE(hidden.app_is_hidden("container1/app6"));
  hidden.add("container1/app6");
  ASSERT_TRUE(hidden.app_is_hidden("container1/app6"));

  QFile file(QString("%1/data/hidden").arg(QDir::currentPath()));
  ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
  auto contents = QString(file.readAll()).split('\n', QString::SkipEmptyParts);
  EXPECT_EQ("container1/app6", contents.last());
}

TEST_F(TestHiddenAppsFixture, AddDoesNotAppendWhenAppAlreadyInFile)
{
  ASSERT_TRUE(hidden.app_is_hidden("container1/app1"));
  hidden.add("container1/app1");

  QFile file(QString("%1/data/hidden").arg(QDir::currentPath()));
  ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
  auto contents = QString(file.readAll()).split('\n', QString::SkipEmptyParts);
  EXPECT_EQ(1, contents.count("container1/app1"));
}

TEST_F(TestHiddenAppsFixture, RemoveDeletesLineFromFile)
{
  ASSERT_FALSE(hidden.app_is_hidden("container1/app6"));
  hidden.add("container1/app6");
  ASSERT_TRUE(hidden.app_is_hidden("container1/app6"));

  hidden.remove("container1/app6");
  ASSERT_FALSE(hidden.app_is_hidden("container1/app6"));

  QFile file(QString("%1/data/hidden").arg(QDir::currentPath()));
  ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
  auto contents = QString(file.readAll()).split('\n', QString::SkipEmptyParts);
  EXPECT_NE("container1/app6", contents.last());
}
}
