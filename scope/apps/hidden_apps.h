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
#ifndef HIDDEN_APPS_H
#define HIDDEN_APPS_H

#include <string>
#include <QStringList>

class HiddenApps
{
public:
  explicit HiddenApps(const std::string& cache_directory);
  virtual ~HiddenApps() = default;

  virtual bool app_is_hidden(QString const& app_id) const;
  virtual bool empty() const;
  virtual void add(QString const& app_id);
  virtual void remove(QString const& app_id);

private:
  QString hidden_file_name_;
  QStringList apps_;
};

#endif // HIDDEN_APPS_H
