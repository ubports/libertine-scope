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
#ifndef LIBERTINE_SCOPE_APPLAUNCHER_H
#define LIBERTINE_SCOPE_APPLAUNCHER_H


#include <string>


/**
 * Information on a container application launcher.
 */
class AppLauncher
{
public:
  /**
   * Constructs an AppLaunchder object from a JSON string.
   */
  explicit
  AppLauncher(const std::string& app_id, const std::string& container_name);

  virtual
  ~AppLauncher();

  virtual std::string
  name() const;

  virtual std::string
  icon() const;

  virtual std::string
  description() const;

  virtual std::string
  uri() const;

protected:
  explicit
  AppLauncher();

  std::string     name_;
  std::string     icon_;
  std::string     uri_;
  std::string     description_;
};

#endif /* LIBERTINE_SCOPE_APPLAUNCHER_H */
