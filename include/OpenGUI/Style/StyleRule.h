#pragma once
#include "OpenGUI/Style/StyleProperty.h"

namespace OGUI
{
	struct StyleRule
	{
		std::vector<StyleProperty> properties;
		std::vector<CustomStyleProperty> customProperties;
	};
}