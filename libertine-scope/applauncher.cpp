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
#include <ubuntu-app-launch/registry.h>


AppLauncher::
AppLauncher(const std::string& app_id, const std::string& container_id)
{
  auto appId = ubuntu::app_launch::AppID::parse(app_id);
  auto app = ubuntu::app_launch::Application::create(appId, ubuntu::app_launch::Registry::getDefault());
  name_ = app->info()->name().value();
  icon_ = app->info()->iconPath().value();
  description_ = app->info()->description().value();
  uri_ = "appid://" + container_id + "/" + appId.appname.value() + "/0.0";
}


AppLauncher::
AppLauncher() { /* empty */ }


AppLauncher::
~AppLauncher()
{
}


std::string AppLauncher::
name() const
{
  return name_;
}


std::string AppLauncher::
icon() const
{
  return icon_;
}


std::string AppLauncher::
description() const
{
  return description_;
}


std::string AppLauncher::
uri() const
{
  return uri_;
}

