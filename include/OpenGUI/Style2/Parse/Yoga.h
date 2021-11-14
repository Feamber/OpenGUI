#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include "yoga/Yoga.h"

namespace OGUI
{
	OGUI_API bool ParseValue(std::string_view str, YGValue& value);
	OGUI_API bool ParseValue(std::string_view str, YGPositionType& value);
	OGUI_API bool ParseValue(std::string_view str, YGOverflow& value);
	OGUI_API bool ParseValue(std::string_view str, YGAlign& value);
	OGUI_API bool ParseValue(std::string_view str, YGFlexDirection& value);
	OGUI_API bool ParseValue(std::string_view str, YGJustify& value);
	OGUI_API bool ParseValue(std::string_view str, YGWrap& value);
	OGUI_API bool ParseValue(std::string_view str, YGDisplay& value);
	OGUI_API bool ParseFourSides(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom);
}