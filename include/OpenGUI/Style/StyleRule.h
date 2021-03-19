#pragma once
#include "OpenGUI/Style/VariantStorage.h"
namespace OGUI
{
	struct StyleRule
	{
		std::vector<VariantHandle> properties;
		int customPropertiesCount;
	};
}