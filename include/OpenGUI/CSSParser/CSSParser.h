#pragma once

#include <string>
#include <optional>
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	std::optional<StyleSheet> ParseCSS(std::string_view str);
	std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
	std::optional<StyleSheet> ParseCSSFile(std::string path);
}