/*
 * Copyright 2015 Canonical Ltd.
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
#include "container.h"

#include <QtCore/QProcess>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QString>


Container::
Container(std::string const& container_id)
: id_(container_id)
, name_(container_id)
{
  QProcess libertine_container_manager;
  libertine_container_manager.start("libertine-container-manager",
                                    QStringList() <<  "list-apps"
                                                  << "--id"
                                                  << QString::fromStdString(id_)
                                                  << "--json");
  if (libertine_container_manager.waitForFinished())
  {
    QJsonDocument json = QJsonDocument::fromJson(libertine_container_manager.readAllStandardOutput());
    QJsonObject object = json.object();
    QJsonValue name = object["name"];
    if (name != QJsonValue::Undefined)
    {
      name_ = name.toString().toStdString();

      QJsonValue v = object["app_launchers"];
      if (v != QJsonValue::Undefined)
      {
        for (auto const& app: v.toArray())
        {
          auto json = QJsonDocument(app.toObject()).toJson().toStdString();
          app_launcher_list_.emplace_back(AppLauncher(json));
        }
      }
    }
  }
}


Container::
~Container()
{
}


std::string Container::
id() const
{
  return id_;
}


std::string Container::
name() const
{
  return name_;
}


Container::AppLauncherList const& Container::
app_launchers() const
{
  return app_launcher_list_;
}

