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
#include <unity/util/IniParser.h>
#include <QFileInfo>
#include <QStringList>


namespace
{
static const std::string DESKTOP_FILE_GROUP("Desktop Entry");
static const std::string DESKTOP_FILE_KEY_COMMENT("Comment");
}


AppDescription::
AppDescription(const std::string& desktop_file, const std::string& locale)
  : desktop_file_(desktop_file), language_(locale)
{
}


AppDescription::
~AppDescription()
{
}


unity::scopes::Variant AppDescription::
description() const
{
  if (QFileInfo(QString::fromStdString(desktop_file_)).exists())
  {
    unity::util::IniParser parser(desktop_file_.c_str());
    if (parser.has_key(DESKTOP_FILE_GROUP, DESKTOP_FILE_KEY_COMMENT))
    {
      return unity::scopes::Variant(parser.get_locale_string(DESKTOP_FILE_GROUP, DESKTOP_FILE_KEY_COMMENT, language_));
    }
  }

  return unity::scopes::Variant("");
}
