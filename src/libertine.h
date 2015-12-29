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
#ifndef LIBERTINE_SCOPE_LIBERTINE_H
#define LIBERTINE_SCOPE_LIBERTINE_H

#include <memory>
#include <vector>


class Container;

/**
 * Proxy object for the Libertine ecosphere.
 */
class Libertine
{
public:
  using ContainerList = std::vector<std::unique_ptr<Container>>;

public:
  Libertine();

  virtual
  ~Libertine() = default;

  /**
   * Gets a list of identifiers for all Libertine containers on the system.
   *
   * This is a blocking call, so it may take a while to complete but it should
   * be fairly fast in most cases.  There is no locking associated with
   * resources so it's possible that the list may not disagree with what's
   * expected due to races if a container is created or destroyed while this
   * function is running.
   */
  virtual ContainerList const&
  get_container_list() const;

private:
  ContainerList container_list_;
};

#endif /* LIBERTINE_SCOPE_LIBERTINE_H */
