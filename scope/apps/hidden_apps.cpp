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

#include <QFile>
#include <QTextStream>


namespace
{
QStringList
get_hidden_apps(const QString& hidden_file_name)
{
  QFile hidden_file(hidden_file_name);
  if (!hidden_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return QStringList{};
  }

  return QString(hidden_file.readAll()).split('\n', QString::SkipEmptyParts);
}
}


HiddenApps::
HiddenApps(const std::string &cache_directory)
  : hidden_file_name_(QString("%1/hidden").arg(QString::fromStdString(cache_directory)))
  , apps_(get_hidden_apps(hidden_file_name_))
{
}


bool HiddenApps::
app_is_hidden(QString const& app_id) const
{
  return apps_.contains(app_id);
}


bool HiddenApps::
empty() const
{
  return apps_.empty();
}


void HiddenApps::
add(const QString &app_id)
{
  if (!app_is_hidden(app_id))
  {
    QFile hidden_file(hidden_file_name_);
    if (hidden_file.open(QIODevice::Append | QIODevice::Text))
    {
      hidden_file.write(app_id.toUtf8() + "\n");
    }

    apps_.append(app_id);
  }
}


void HiddenApps::
remove(const QString &app_id)
{
  if (app_is_hidden(app_id))
  {
    apps_.removeAll(app_id);

    QFile hidden_file(hidden_file_name_);
    if (hidden_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      for (auto const& app : apps_)
      {
        hidden_file.write(app.toUtf8() + "\n");
      }
    }
  }
}
