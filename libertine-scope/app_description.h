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

#ifndef APP_DESCRIPTION_H
#define APP_DESCRIPTION_H

#include <string>

namespace unity {
  namespace scopes {
    class Variant;
  }
}

class AppDescription
{
public:
  explicit
  AppDescription(const std::string& desktop_file, const std::string& locale);
  ~AppDescription();
  unity::scopes::Variant description() const;

private:
  std::string desktop_file_;
  std::string language_;
};

#endif
