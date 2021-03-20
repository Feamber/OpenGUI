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
	struct Dimension
	{
		enum
		{
			Unitless,
			Pixel,
			Percent
		} unit;

		float value;
	};

	struct StyleSheet
	{
		VariantStorage<
			float,
			Color4f,
			std::string,
			int,
			Dimension
			> Storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;
	};
}