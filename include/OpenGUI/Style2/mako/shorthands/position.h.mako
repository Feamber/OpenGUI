#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/position.h"
<%namespace name="helpers" file="/shorthands/helper.h.mako" />

${helpers.four_sides_shorthand(
    "margin",
    "margin-%s",
)}

${helpers.four_sides_shorthand(
    "padding",
    "padding-%s",
)}