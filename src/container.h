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
#ifndef LIBERTINE_SCOPE_CONTAINER_H
#define LIBERTINE_SCOPE_CONTAINER_H


#include "applauncher.h"
#include <string>
#include <vector>


/**
 * Proxy object for a Libertine container.
 */
class Container
{
public:
  using AppLauncherList = std::vector<AppLauncher>;

public:
  /**
   * Constructs an identified container.
   *
   * Synchronously scans the Libertine container for eligible application
   * launcher .desktop files and their icons.
   */
  explicit
  Container(std::string const& container_id);

  virtual
  ~Container();

  virtual std::string
  id() const;

  virtual std::string
  name() const;

  virtual AppLauncherList const&
  app_launchers() const;

private:
  std::string     id_;
  std::string     name_;
  AppLauncherList app_launcher_list_;
};

#endif /* LIBERTINE_SCOPE_CONTAINER_H */
