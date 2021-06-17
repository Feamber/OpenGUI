#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include <tuple>
#include <string>
#include <map>
#include <string_view>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleRule.h"
#include "StyleSelector.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Animation/AnimTypes.h"

namespace OGUI
{
	struct AnimTimingFunction;
	using StyleSheetStorage = VariantStorage<
		float,
		Color4f,
		int,
		YGValue,
		std::string,
		AnimTimingFunction,
		Vector2f
		>;

	struct StyleKeyframes
	{
		struct Key
		{
			float percentage;
			int frameIndex;
		};
		std::string name;
		std::vector<Key> keys;
	};

	struct StyleSheet
	{
		std::string path;

		StyleSheetStorage storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;
		std::vector<StyleKeyframes> styleKeyframes;

		using SelectorMap = std::multimap<std::string_view, int>;
		SelectorMap classSelectors;
		SelectorMap nameSelectors;
		SelectorMap typeSelectors;

		using KeyframesMap = std::map<std::string_view, int>;
		KeyframesMap namedKeyframes;

		OGUI_API void Initialize();
	};

	struct InlineStyle
	{
		StyleRule rule;
		StyleSheetStorage storage;
	};

	enum class ParseErrorType
	{
		None,
		InvalidProperty,
		InvalidValue
	};
	bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view str, StyleRule& rule, const char*& errorMsg, ParseErrorType& errorType, int animIndex = -1, int animEndIndex = -1);
}