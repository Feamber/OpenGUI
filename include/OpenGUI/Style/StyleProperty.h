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
#undef	STYLEPROP

#define	ANIMPROP(name, ...)\
		name,
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
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