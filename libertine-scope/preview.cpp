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

#include "libertine-scope/preview.h"
#include "libertine-scope/app_description.h"

#include <unity/scopes/PreviewReply.h>
#include <unity/scopes/Variant.h>
#include <unity/scopes/VariantBuilder.h>
#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/Result.h>

namespace usc = unity::scopes;

Preview::
Preview(usc::Result const&         result,
        usc::ActionMetadata const& metadata)
: PreviewQueryBase(result, metadata)
{
}


Preview::
~Preview()
{
}


void Preview::
cancelled()
{
}


void Preview::
run(usc::PreviewReplyProxy const& reply)
{
  usc::PreviewWidget header("hdr", "header");
  header.add_attribute_mapping("title", "title");
  header.add_attribute_mapping("mascot", "art");

  usc::PreviewWidget buttons("buttons", "actions");
  usc::VariantBuilder vb;
  vb.add_tuple({
    {"id",    usc::Variant("open")},
    {"label", usc::Variant("Open")},
  });
  buttons.add_attribute_value("actions", vb.end());

  auto desktop_file_name = result()["desktop_file"].get_string();
  usc::PreviewWidget desc("desc", "text");
  desc.add_attribute_value("text", AppDescription(desktop_file_name, action_metadata().locale()).description());

  usc::PreviewWidgetList widgets{ header, desc, buttons };
  reply->push(widgets);
}
