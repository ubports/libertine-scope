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
#include "tests/fake_libertine.h"

#include "tests/fake_container.h"


FakeLibertine::
FakeLibertine(std::string const& json)
{
  container_list_.emplace_back(new FakeContainer(json));
}


Libertine::ContainerList const& FakeLibertine::
get_container_list() const
{
  return container_list_;
}


Libertine::UPtr FakeLibertine::
make_fake(std::string const& json)
{
  return Libertine::UPtr(new FakeLibertine(json));
}

