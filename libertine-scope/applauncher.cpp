/*
 * Copyright 2015-2016 Canonical Ltd.
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
#include "libertine-scope/applauncher.h"

#include <QtCore/QByteArray>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>


AppLauncher::
AppLauncher(std::string const& json_string)
{
  auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(json_string));
  auto obj = doc.object();

  name_ = obj["name"].toString().toStdString();

  no_display_ = obj["no_display"].toBool();

  QJsonValue icons = obj["icons"];
  if (icons.isArray())
  {
    int width = 0;
    for (auto const& icon: icons.toArray())
    {
      QString icon_file_name = icon.toString();
      if (icon_file_name.endsWith(".svg", Qt::CaseInsensitive))
      {
        icon_ = "file://" + icon_file_name.toStdString();
        break;
      }

      QImage image = QImage(icon_file_name);
      if (image.width() > width)
      {
        icon_ = "file://" + icon_file_name.toStdString();
        width = image.width();
      }
    }
  }

  desktop_file_ = obj["desktop_file_name"].toString().toStdString();
}


AppLauncher::
~AppLauncher()
{
}


std::string AppLauncher::
id() const
{
  QFileInfo fi(QString::fromStdString(desktop_file_));
  return fi.baseName().toStdString();
}


std::string AppLauncher::
name() const
{
  return name_;
}


bool AppLauncher::
no_display() const
{
  return no_display_;
}


std::string AppLauncher::
icon() const
{
  return icon_;
}


std::string AppLauncher::
desktop_file() const
{
  return desktop_file_;
}

