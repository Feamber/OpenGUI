#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <map>
#include <string_view>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleRule.h"
#include "StyleSelector.h"
#include "yoga/Yoga.h"

namespace OGUI
{
	struct Picture
	{
		std::string path;
		int loadingStatus;
		std::shared_ptr<ITexture> texture;
	};
	using StyleSheetStorage = VariantStorage<
		float,
		Color4f,
		int,
		YGValue,
		Picture
		>;

	struct StyleSheet
	{
		StyleSheetStorage storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;

		using SelectorMap = std::multimap<std::string_view, int>;
		SelectorMap classSelectors;
		SelectorMap nameSelectors;
		SelectorMap typeSelectors;

		void Initialize();
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
	bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view str, StyleRule& rule, const char*& errorMsg, ParseErrorType& errorType);
}