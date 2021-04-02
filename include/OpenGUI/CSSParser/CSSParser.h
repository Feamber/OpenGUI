#pragma once
#include <string>
#include <optional>
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	OGUI_API std::optional<StyleSheet> ParseCSS(std::string_view str);
	OGUI_API std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
	OGUI_API std::optional<StyleSheet> ParseCSSFile(std::string path);

	using property_list_t = std::vector<std::pair<std::string_view, std::string_view>>;
	OGUI_API void sort(property_list_t& list);
}