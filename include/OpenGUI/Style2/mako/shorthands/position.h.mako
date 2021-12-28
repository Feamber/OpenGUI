#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/position.h"
<%!
   from tool.style_codegen import (LOGICAL_CORNERS, PHYSICAL_CORNERS, LOGICAL_SIDES,
                      PHYSICAL_SIDES, LOGICAL_SIZES, LOGICAL_AXES)
%>
<%namespace name="helpers" file="/shorthands/helper.h.mako" />

${helpers.four_sides_shorthand(
    "margin",
    "margin-%s",
    PHYSICAL_SIDES
)}

${helpers.four_sides_shorthand(
    "padding",
    "padding-%s",
    PHYSICAL_SIDES
)}