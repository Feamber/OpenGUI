#pragma once
#include <vector>
#include "OpenGUI/Core/Containers/span.hpp"

namespace OGUI
{
	struct AnimStyle;
	struct AnimRunContext;
	struct StyleSheetStorage;
	struct StyleRule;
	struct StyleSheet;
	struct StyleKeyframes;
    struct ComputedStyle;

	template<class T>
	std::vector<T> ToOwned(gsl::span<T> s)
	{
		return {s.begin(), s.end()};
	}
}