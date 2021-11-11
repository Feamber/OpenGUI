#pragma once
#include <string_view>
#include "yoga/Yoga.h"

namespace OGUI
{
	bool ParseValue(std::string_view str, YGValue& value);
	bool ParseValue(std::string_view str, YGPositionType& value);
	bool ParseValue(std::string_view str, YGOverflow& value);
	bool ParseValue(std::string_view str, YGAlign& value);
	bool ParseValue(std::string_view str, YGFlexDirection& value);
	bool ParseValue(std::string_view str, YGJustify& value);
	bool ParseValue(std::string_view str, YGWrap& value);
	bool ParseValue(std::string_view str, YGDisplay& value);
	bool ParseFourSides(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom);
}