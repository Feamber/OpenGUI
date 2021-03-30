#pragma once
#include "OpenGUI/CSSParser/CSSParser.h"

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

namespace OGUI
{
	bool FromString(std::string_view str, std::string& value);
	bool FromString(std::string_view str, int& value);
	bool FromString(std::string_view str, YGValue& value);
	bool FromString(std::string_view str, AnimTimingFunction& TimingFunction);
	bool FromColorName(std::string_view str, Color4f& value);
	float FromAngle(std::string_view str);
	float FromTranslationComponent(std::string_view str);
	bool FromTranslation(std::string_view str, Vector2f& t);
	bool FromString(std::string_view str, Vector2f& t);
	bool FromString(std::string_view str, Color4f& value);
	bool FromString(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom);
	bool FromTime(std::string_view str, float& value);
	bool FromTime(std::string_view str, std::vector<float>& values);
	bool FromString(std::string_view str, YGPositionType& value);
	bool FromString(std::string_view str, YGOverflow& value);
	bool FromString(std::string_view str, YGAlign& value);
	bool FromString(std::string_view str, YGFlexDirection& value);
	bool FromString(std::string_view str, YGJustify& value);
	bool FromString(std::string_view str, YGWrap& value);
	bool FromString(std::string_view str, YGDisplay& value);
	bool FromString(std::string_view str, EAnimDirection& value);
	bool FromString(std::string_view str, EAnimResumeMode& value);
	bool FromString(std::string_view str, EAnimYieldMode& value);
	bool FromString(std::string_view str, EAnimFillMode& value);
	bool FromUrl(std::string_view str, std::string& value);
	bool FromIterationCount(std::string_view str, float& value);
	bool FromString(std::string_view str, StyleKeyword& value);
	bool FromTransform(std::string_view str, StyleSheetStorage& sheet, StyleRule& rule);
	
	template<class T>
	bool FromString(std::string_view str, T& value)
	{
		std::cerr << "value type [" << typeid(T).name() << "] is not supported!\n";
		return false;
	}
	template<class T>
	bool FromString(std::string_view str, std::vector<T>& values)
	{
		std::vector<std::string_view> tokens;
		std::split(str, tokens, ", ");
		for (auto& token : tokens)
		{
			T value;
			if (!FromString(token, value))
				return false;
			values.push_back(std::move(value));
		}
		return true;
	}
}