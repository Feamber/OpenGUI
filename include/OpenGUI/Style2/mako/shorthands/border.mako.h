#pragma once
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/border.h"
<%namespace name="helpers" file="/shorthands/helper.mako.h" />

${helpers.four_corners_shorthand(
    "border-radius",
    "border-%s-radius",
)}