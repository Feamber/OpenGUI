#pragma once
#include <vector>
#include <tuple>
#include <string>
#include "boost/hana.hpp"
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleRule.h"
#include "StyleSelector.h"

namespace OGUI
{
	

	struct StyleSheet
	{
		VariantStorage<
			float,
			Color4f,
			std::string,
			int
			> Storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;
		void Initialize();
	};
}