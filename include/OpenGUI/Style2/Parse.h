#pragma once
#include <string>
#include <optional>
#include "OpenGUI/Style2/Rule.h"
#include "AnimTypes.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/ParseUtils.hpp"

namespace OGUI
{
    using PropertyList = std::vector<std::pair<std::string_view, std::string_view>>;
	
    OGUI_API StyleFont ParseFontFace(const PropertyList& props);
	OGUI_API std::optional<StyleSheet> ParseCSS(std::string_view str);
	OGUI_API std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
	OGUI_API std::optional<StyleSheet> ParseCSSFile(std::string path);
    OGUI_API bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount = -1);
    using PropertyParseFunc = bool(*)(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg);
}