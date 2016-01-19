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
#ifndef LIBERTINE_SCOPE_FAKE_CONTAINER_JSON_H_
#define LIBERTINE_SCOPE_FAKE_CONTAINER_JSON_H_

#include <string>

const std::string FAKE_CONTAINER_JSON = R"(
{
    "name": "Fake Container",
    "app_launchers": [
        {
            "desktop_file_name": "/home/someuser/.cache/libertine-container/fake1/rootfs/usr/share/applications/mb-panel-manager.desktop",
            "exec_line": "matchbox-panel-manager",
            "icons": [
                "/home/someuser/.cache/libertine-container/fake1/rootfs/usr/share/pixmaps/mbpanelmgr.png"
            ],
            "mime_types": [],
            "name": "Panel Manager",
            "no_display": false
        },
        {
            "desktop_file_name": "/home/someuser/.cache/libertine-container/fake1/rootfs/usr/share/applications/sakura.desktop",
            "exec_line": "sakura",
            "icons": [
                "/home/someuser/.cache/libertine-container/fake1/rootfs/usr/share/pixmaps/terminal-tango.svg"
            ],
            "mime_types": [],
            "name": "Sakura",
            "no_display": false
        }
    ]
}
)";

#endif /* LIBERTINE_SCOPE_FAKE_CONTAINER_JSON_H_ */
