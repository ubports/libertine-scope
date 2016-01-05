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
#include "libertine.h"

#include "container.h"
#include <QtCore/QProcess>
#include <QtCore/QString>


Libertine::
Libertine()
{
  QProcess libertine_container_manager;
  libertine_container_manager.start("libertine-container-manager",
                                    QStringList() <<  "list");
  if (libertine_container_manager.waitForFinished())
  {
    QString container_id_list(libertine_container_manager.readAllStandardOutput());
    for (auto const& id: container_id_list.split("\n", QString::SkipEmptyParts))
    {
      container_list_.emplace_back(new Container(id.toStdString()));
    }
  }
}


Libertine::ContainerList const& Libertine::
get_container_list() const
{
  return container_list_;
}

