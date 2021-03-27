#include "OpenGUI/Animation/StyleAnimation.h"
#include <algorithm>

namespace OGUI
{
	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, T>
	Lerp(const T& a, const T& b, float alpha)
	{
		return a * (1 - alpha) + b * alpha;
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, T>
	Lerp(const T& a, const T& b, float alpha)
	{
		return b;
	}

	YGValue Lerp(const YGValue& a, const YGValue& b, float alpha)
	{
		assert(a.unit == b.unit);
		return {Lerp(a.value, b.value, alpha), b.unit};
	}
}

OGUI::Style OGUI::Lerp(const Style& a, const Style& b, float alpha)
{
	alpha = std::clamp(alpha, 0.f, 1.f);
	if (alpha == 0.f)
		return a;
	if (alpha == 1.f)
		return b;
	Style result;
#define STYLEPROP(name, ...) \
	result.name = Lerp(a.name, b.name, alpha);
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef STYLEPROP
	return result;
}
