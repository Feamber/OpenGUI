#pragma once
#include <string>
#include <string_view>
#include <bitset>
#include <gsl/span>
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style/VariantStorage.h"
#include "OpenGUI/Style/StylePropertiesDef.h"
#include "OpenGUI/Animation/AnimPropertiesDef.h"

namespace OGUI
{

	enum class StyleKeyword : int
	{
		Inherit,
		Initial,
		Auto,
		Unset,
		True,
		False,
		None
	};

	enum class StylePropertyId : uint32_t
	{
#define	GEN(name, ...)\
		name,
		STYLEPROP(GEN)
		ANIMPROP(GEN)
#undef GEN
		_End,
		NumStyle = animName,
		NumAnim = _End - NumStyle,
		Num = _End - 1,
	};

	struct StyleProperty
	{
		StylePropertyId id;
		VariantHandle value;
		bool keyword : 1;
		int arrayIndex : 31;
		StyleProperty() = default;
		StyleProperty(StylePropertyId id, VariantHandle handle, int arrayIndex = -1)
			: id(id), value(handle), keyword(false), arrayIndex(arrayIndex) {}
		StyleProperty(StylePropertyId id, int keyword, int arrayIndex = -1)
			: id(id), value{keyword}, keyword(true), arrayIndex(arrayIndex)
		{}
	};

	struct CustomStyleProperty
	{
		std::string name;
		int Type : 31;
		bool Keyword : 1;
		VariantHandle value;
	};

	OGUI_API StylePropertyId PropertyNameToId(std::string_view name);
	OGUI_API std::string_view PropertyIdToName(StylePropertyId id);
	OGUI_API int PropertyNameToOrder(std::string_view name);
	OGUI_API std::bitset<96> GetInheritMask();
	OGUI_API void GetOverrideMask(const gsl::span<StyleProperty>& props, std::bitset<96>& ovr, std::bitset<96>& iht);
}