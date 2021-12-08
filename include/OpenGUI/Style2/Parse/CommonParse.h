#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include "OpenGUI/Style2/Properties.h"

namespace OGUI
{
    OGUI_API bool ParseValue(std::string_view str, std::string& value);
	OGUI_API bool ParseUrl(std::string_view str, std::string& value);
	OGUI_API bool ParseValue(std::string_view str, StyleKeyword& value);
}