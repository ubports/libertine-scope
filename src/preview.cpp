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
#include "preview.h"

#include <unity/scopes/PreviewReply.h>
#include <unity/scopes/Variant.h>
#include <unity/scopes/VariantBuilder.h>


Preview::
Preview(unity::scopes::Result const&         result,
        unity::scopes::ActionMetadata const& metadata)
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
run(unity::scopes::PreviewReplyProxy const& reply)
{
  reply->push("description", unity::scopes::Variant("A Description"));

  unity::scopes::PreviewWidget image_widget("myimage", "image");
  image_widget.add_attribute_mapping("source", "art");

  unity::scopes::PreviewWidget buttons("buttons", "actions");
  unity::scopes::VariantBuilder vb;
  vb.add_tuple({
    {"id",    unity::scopes::Variant("open")},
    {"label", unity::scopes::Variant("Open")},
  });
  buttons.add_attribute_value("actions", vb.end());

  unity::scopes::PreviewWidgetList widgets{ image_widget, buttons };
  reply->push(widgets);
}

