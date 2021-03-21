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

	using StyleSheetStorage = VariantStorage<
		float,
		Color4f,
		int,
		YGValue
		>;

	struct StyleSheet
	{
		StyleSheetStorage Storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;

		using SelectorMap = std::multimap<std::string_view, StyleComplexSelector*>;
		SelectorMap classSelectors;
		SelectorMap nameSelectors;
		SelectorMap typeSelectors;

		void Initialize();
	};
}