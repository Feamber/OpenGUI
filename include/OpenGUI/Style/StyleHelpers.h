#pragma once
#include "OpenGUI/Style/StyleRule.h"
#include "OpenGUI/Style/StyleSheet.h"
namespace OGUI
{
	namespace detail
	{
		template<class T, class Y>
		std::enable_if_t<std::is_convertible_v<T, Y>&&!std::is_enum_v<T>, StyleProperty>
			AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const Y& value)
		{
			auto handle = sheet.Push<T>(value);
			return {id, false, handle};
		}

		template<class T, class Y>
		std::enable_if_t<std::is_convertible_v<T, Y>&&std::is_enum_v<T>, StyleProperty>
			AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const Y& value)
		{
			auto handle = sheet.Push<int>(value);
			return {id, false, handle};
		}

		template<class T, class Y>
		std::enable_if_t<!std::is_convertible_v<T, Y>, StyleProperty>
			AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const Y& value)
		{
			//assert(false);
			return {};
		}
	}

	template<class T>
	StyleProperty AddProperty(StyleSheetStorage& sheet, StylePropertyId id, const T& value)
	{
#define STYLEPROP(name, index, inherit, type, ...)\
		if(id == StylePropertyId::name) \
		{ \
			return detail::AddPropertyImpl<type>(sheet, id, value); \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
		//assert(false);
		return {};
	}
}