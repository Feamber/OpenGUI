#pragma once
#include <string>
#include <optional>
#include "OpenGUI/Style2/Rule.h"
#include "AnimTypes.h"
#include "OpenGUI/Core/OMath.h"

namespace OGUI
{
	OGUI_API std::optional<StyleSheet> ParseCSS(std::string_view str);
	OGUI_API std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
	OGUI_API std::optional<StyleSheet> ParseCSSFile(std::string path);
    OGUI_API bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount = -1);
    using PropertyParseFunc = bool(*)(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg);
}

namespace std
{
	inline void split(const string_view& s, vector<string_view>& tokens, const string_view& delimiters = " ")
	{
		string::size_type lastPos = s.find_first_not_of(delimiters, 0);
		string::size_type pos = s.find_first_of(delimiters, lastPos);
		while (string::npos != pos || string::npos != lastPos)
		{
			auto substr = s.substr(lastPos, pos - lastPos);
			tokens.push_back(substr);//use emplace_back after C++11
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
		}
	}

	inline bool ends_with(std::string_view const& value, std::string_view const& ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	inline bool starts_with(std::string_view const& value, std::string_view const& starting)
	{
		if (starting.size() > value.size()) return false;
		return std::equal(starting.begin(), starting.end(), value.begin());
	}
}