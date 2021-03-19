#pragma once
#include <vector>
#include <tuple>
#include <string>
#include "boost/hana.hpp"
#include "Core/Math.h"
#include "VariantStorage.h"

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
		//StyleRules
		//Selectors
	};
}