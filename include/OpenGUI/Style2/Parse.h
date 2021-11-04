#pragma once
#include <string>
#include <optional>
#include "OpenGUI/Style2/Rule.h"
#include "yoga/Yoga.h"
#include "AnimTypes.h"
#include "OpenGUI/Core/Math.h"

namespace OGUI
{
	OGUI_API std::optional<StyleSheet> ParseCSS(std::string_view str);
	OGUI_API std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
	OGUI_API std::optional<StyleSheet> ParseCSSFile(std::string path);
    OGUI_API bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg, int animCount = -1);
    using PropertyParseFunc = bool(*)(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);

    bool ParseValue(std::string_view str, std::string& value);
	bool ParseValue(std::string_view str, float& value);
	bool ParseValue(std::string_view str, int& value);
	bool ParseValue(std::string_view str, YGValue& value);
	bool ParseValue(std::string_view str, struct AnimTimingFunction& TimingFunction);
	bool ParseColorName(std::string_view str, Color4f& value);
	bool ParseTranslate(std::string_view str, Vector2f& t);
	bool ParseValue(std::string_view str, Vector2f& t);
	bool ParseValue(std::string_view str, Color4f& value);
	bool ParseFourSides(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom);
	bool ParseTime(std::string_view str, float& value);
	bool ParseTime(std::string_view str, std::vector<float>& values);
	bool ParseValue(std::string_view str, YGPositionType& value);
	bool ParseValue(std::string_view str, YGOverflow& value);
	bool ParseValue(std::string_view str, YGAlign& value);
	bool ParseValue(std::string_view str, YGFlexDirection& value);
	bool ParseValue(std::string_view str, YGJustify& value);
	bool ParseValue(std::string_view str, YGWrap& value);
	bool ParseValue(std::string_view str, YGDisplay& value);
	bool ParseValue(std::string_view str, EAnimDirection& value);
	bool ParseValue(std::string_view str, EAnimResumeMode& value);
	bool ParseValue(std::string_view str, EAnimYieldMode& value);
	bool ParseValue(std::string_view str, EAnimFillMode& value);
	bool ParseUrl(std::string_view str, std::string& value);
	bool ParseIterationCount(std::string_view str, float& value);
	bool ParseValue(std::string_view str, StyleKeyword& value);
	bool ParseTransform(std::string_view str, StyleSheetStorage& sheet, StyleRule& rule);
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