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
  AppLauncher(std::string const& json_string);

  virtual
  ~AppLauncher();

  virtual std::string
  name() const;

  virtual bool
  no_display() const;

  virtual std::string
  icon() const;

  virtual std::string
  desktop_file() const;

private:
  std::string     name_;
  bool            no_display_;
  std::string     icon_;
  std::string     desktop_file_;
};

#endif /* LIBERTINE_SCOPE_APPLAUNCHER_H */
