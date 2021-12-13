#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include "OpenGUI/Text/TextTypes.h"
#include "OpenGUI/Style2/Properties.h"

namespace OGUI
{
	OGUI_API bool ParseValue(std::string_view str, TextStyle& value);
	OGUI_API bool ParseValue(std::string_view str, TextAlign& value);
	OGUI_API bool ParseTextWeight(std::string_view str, int& value);
    OGUI_API bool ParseLineHeight(std::string_view str, float& value);
}