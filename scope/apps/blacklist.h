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
#ifndef BLACKLIST_H
#define BLACKLIST_H

#include <QStringList>

class Blacklist
{
public:
  explicit Blacklist(std::string const& data_directory);
  virtual ~Blacklist() = default;

  virtual bool app_is_blacklisted(QString const& app_id, std::string const& container_id) const;

private:
  void parse_blacklist(QString const& blacklist_file_name);

  QStringList blacklist_;
  QStringList whitelist_;
};

#endif // BLACKLIST_H
