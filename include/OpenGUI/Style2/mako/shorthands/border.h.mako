#pragma once
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/border.h"
<%namespace name="helpers" file="/shorthands/helper.h.mako" />
<%!
   from tool.style_codegen import (LOGICAL_CORNERS, PHYSICAL_CORNERS, LOGICAL_SIDES,
                      PHYSICAL_SIDES, LOGICAL_SIZES, LOGICAL_AXES)
%>

${helpers.four_sides_shorthand(
    "border-radius",
    "border-%s-radius",
    PHYSICAL_CORNERS,
)}