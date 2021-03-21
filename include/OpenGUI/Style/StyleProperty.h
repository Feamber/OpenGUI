#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/Style/VariantStorage.h"

namespace OGUI
{
	enum Inheritance
	{
		Inherited,
		NonInherited
	};
	enum class StylePropertyId : uint32_t
	{
#define	STYLEPROP(name, index, ...)\
		name = index,
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
		Num,
	};

	struct StyleProperty
	{
		StylePropertyId id : 31;
		bool keyword : 1;
		VariantHandle value;
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
}