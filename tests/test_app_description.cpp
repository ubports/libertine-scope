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

#include "libertine-scope/app_description.h"
#include <unity/scopes/Variant.h>
#include <gtest/gtest.h>
#include <QDir>


TEST(TestAppDescription, returnsEmptyStringForEmptyNamedFile)
{
	AppDescription desc("", "");
	EXPECT_EQ("", desc.description().get_string());
}

TEST(TestAppDescription, returnsEmptyStringForNonExistentDesktopFile)
{
	AppDescription desc("/non/existent/file", "");
	EXPECT_EQ("", desc.description().get_string());
}

TEST(TestAppDescription, returnsEmptyStringForDesktopFileWithoutComment)
{
	auto data_path = QString("%1/data/NoComment.desktop").arg(QDir::currentPath()).toStdString();
	AppDescription desc(data_path, "");
	EXPECT_EQ("", desc.description().get_string());
}

TEST(TestAppDescription, returnsTopLevelCommentWhenNoLocaleSpecified)
{
	auto data_path = QString("%1/data/Full.desktop").arg(QDir::currentPath()).toStdString();
	AppDescription desc(data_path, "");
	EXPECT_EQ("This session logs you into Matchbox", desc.description().get_string());
}

TEST(TestAppDescription, returnsTopLevelCommentWhenLocaleNotAvailable)
{
	auto data_path = QString("%1/data/Full.desktop").arg(QDir::currentPath()).toStdString();
	AppDescription desc(data_path, "fr_FR");
	EXPECT_EQ("This session logs you into Matchbox", desc.description().get_string());
}

TEST(TestAppDescription, returnsLocaleBasedComment)
{
	auto data_path = QString("%1/data/Full.desktop").arg(QDir::currentPath()).toStdString();
	AppDescription desc(data_path, "de_DE");
	EXPECT_EQ("Im Internet surfen", desc.description().get_string());
}
