#pragma once
#include <string>
#include <string_view>
#include <bitset>
#include <gsl/span>
#include "OpenGUI/Style/VariantStorage.h"

namespace OGUI
{
	enum Inheritance
	{
		Inherited,
		NonInherited
	};

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
#define	STYLEPROP(name, ...)\
		name,
#include "OpenGUI/Style/StylePropertiesDef.h"
		NumStyle,
		__ = NumStyle - 1,
#define	ANIMPROP(name, ...)\
		name,
#include "OpenGUI/Animation/AnimPropertiesDef.h"
		_End,
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
	StylePropertyId PropertyNameToId(std::string_view name);
	std::string_view PropertyIdToName(StylePropertyId id);
	int PropertyNameToOrder(std::string_view name);
	std::bitset<96> GetInheritMask();

	void GetOverrideMask(const gsl::span<StyleProperty>& props, std::bitset<96>& ovr, std::bitset<96>& iht);
}