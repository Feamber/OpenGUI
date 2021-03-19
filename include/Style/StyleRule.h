#pragma once
#include "VariantStorage.h"
namespace OGUI
{
	struct StyleRule
	{
		std::vector<VariantHandle> properties;
		int customPropertiesCount;
	};
}