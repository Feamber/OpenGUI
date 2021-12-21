#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include "OpenGUI/Core/Types.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Properties.h"

namespace OGUI
{
	OGUI_API bool ParseValue(std::string_view str, YGValue& value);
	OGUI_API bool ParseValue(std::string_view str, YGPositionType& value);
	OGUI_API bool ParseValue(std::string_view str, StyleOverflow& value);
	OGUI_API bool ParseValue(std::string_view str, YGAlign& value);
	OGUI_API bool ParseValue(std::string_view str, YGFlexDirection& value);
	OGUI_API bool ParseValue(std::string_view str, YGJustify& value);
	OGUI_API bool ParseValue(std::string_view str, YGWrap& value);
	OGUI_API bool ParseValue(std::string_view str, YGDisplay& value);
	OGUI_API bool ParseValue(std::string_view str, EInlineAlign& value);
	OGUI_API bool ParseFourSides(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom);
	OGUI_API bool ParseRatio(std::string_view str, float& value);
}