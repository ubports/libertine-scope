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

#include "scope/apps/libertine.h"
#include "scope/apps/container.h"
#include <liblibertine/libertine.h>
#include <glib.h>

namespace
{

/**
 * A real Libertine Container created by querying liblibertine.
 */
class LibertineContainer
: public Container
{
public:
  LibertineContainer(std::string const& container_id, std::string const& container_name)
  : Container(container_id, container_name)
  {
    auto apps = libertine_list_apps_for_container(container_id.c_str());
    for (auto i = 0; apps[i] != nullptr; ++i)
    {
      try
      {
        app_launcher_list_.emplace_back(AppLauncher(apps[i], container_id));
      }
      catch(std::runtime_error)
      {
        /*
         * U-A-L will throw if an application cannot be displayed
         * for any reason. Ignore and continue.
         */
      }
    }
    g_strfreev(apps);
  }

  ~LibertineContainer()
  { }
};

class LibertineCli
: public Libertine
{
public:
  LibertineCli()
  {
    auto container_ids = libertine_list_containers();
    for (auto i = 0; container_ids[i] != nullptr; ++i)
    {
      auto name = libertine_container_name(container_ids[i]);
      container_list_.emplace_back(new LibertineContainer(container_ids[i], name));
      g_free(name);
    }
    g_strfreev(container_ids);
  }

  Libertine::ContainerList const&
  get_container_list() const override
  {
    return container_list_;
  }

private:
  ContainerList container_list_;
};

} // anonymous namespace


Libertine::
~Libertine()
{ }


Libertine::UPtr Libertine::
from_libertine_cli()
{
  return Libertine::UPtr(new LibertineCli());
}

