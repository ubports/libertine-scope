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

#include <QFile>
#include <QTextStream>

namespace
{
inline QString
parse_whitelist_key(QString const& line)
{
  QStringList props = line.split("/");
  return props.length() == 3 ? QString("%1/%2").arg(props[1]).arg(props[2]) : "";
}
}


Blacklist::
Blacklist(std::string const& data_directory)
{
  parse_blacklist(QString("%1/blacklist").arg(QString::fromStdString(data_directory)));
}


void Blacklist::
parse_blacklist(QString const& blacklist_file_name)
{
  QFile blacklist_file(blacklist_file_name);
  if (blacklist_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream in(&blacklist_file);
    while (!in.atEnd())
    {
      QString line(in.readLine());
      if (!line.startsWith("#"))
      {
        if (line.startsWith("whitelist"))
        {
          auto whitelisted_app = parse_whitelist_key(line);
          if (!whitelisted_app.isEmpty())
          {
            whitelist_.append(parse_whitelist_key(line));
          }
        }
        else
        {
          blacklist_.append(line.trimmed());
        }
      }
    }
    blacklist_file.close();
  }
}


bool Blacklist::
app_is_blacklisted(QString const& app_id, std::string const& container_id) const
{
  auto global_app_id = QString("all/%1").arg(app_id);
  auto local_app_id = QString("%1/%2").arg(QString::fromStdString(container_id)).arg(app_id);

  return !(whitelist_.contains(global_app_id) || whitelist_.contains(local_app_id)) &&
          (blacklist_.contains(global_app_id) || blacklist_.contains(local_app_id));
}
