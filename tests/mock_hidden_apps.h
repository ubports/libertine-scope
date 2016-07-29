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
#include "scope/apps/hidden_apps.h"
#include <gmock/gmock.h>


class MockHiddenApps : public HiddenApps
{
public:
  MockHiddenApps()
    : HiddenApps("")
  {
  }

  MOCK_CONST_METHOD1(app_is_hidden, bool(QString const&));
  MOCK_CONST_METHOD0(empty, bool());
  MOCK_METHOD1(remove, void(QString const&));
  MOCK_METHOD1(add, void(QString const&));
};
