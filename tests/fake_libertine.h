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
#ifndef LIBERTINE_SCOPE_FAKE_LIBERTINE_H_
#define LIBERTINE_SCOPE_FAKE_LIBERTINE_H_


#include "libertine-scope/libertine.h"


/**
 * A Fake Libertine.
 */
class FakeLibertine
: public Libertine
{
public:
  virtual ~FakeLibertine() = default;

  virtual Libertine::ContainerList const&
  get_container_list() const override;

  static Libertine::UPtr
  make_fake(std::string const& json);

protected:
  explicit
  FakeLibertine(std::string const& json);

private:
  ContainerList container_list_;
};

#endif /* LIBERTINE_SCOPE_FAKE_LIBERTINE_H_ */
