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

#include "scope/apps/container.h"


Container::
Container(std::string const& container_id, std::string const& container_name)
: id_(container_id)
, name_(container_name)
{ }


Container::
~Container()
{ }


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

