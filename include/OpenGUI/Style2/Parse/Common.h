#pragma once
#include <string_view>
#include "OpenGUI/Style2/Properties.h"

namespace OGUI
{
    bool ParseValue(std::string_view str, std::string& value);
	bool ParseUrl(std::string_view str, std::string& value);
	bool ParseValue(std::string_view str, StyleKeyword& value);
}