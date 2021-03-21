#include "OpenGUI/Style/Style.h"
#include <yoga/YGNode.h>

OGUI::Style OGUI::Style::Create(Style* parent, bool isShared)
{
	Style style;
	style.isShared = isShared;
	if (parent)
		InheritData(*parent);
	return style;
}

void OGUI::Style::InheritData(Style& parent)
{
#define STYLEPROP(name, index, inherit, ...)\
	if constexpr(inherit == Inherited) \
	{ \
		name = parent.name; \
	}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
}

template<class T>
std::enable_if_t<!std::is_enum_v<T>, void> 
ApplyProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
{
	field = sheet.Get<T>(prop.value.index);
}

template<class T>
std::enable_if_t<std::is_enum_v<T>, void>
ApplyProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
{
	field = sheet.Get<int>(prop.value.index);
}

void OGUI::Style::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
	for (auto& prop : props)
	{
		if (ApplyGlobalKeyword(prop))
			continue;
#define STYLEPROP(name, index, inherit, type, ...)\
		if(prop.id == StylePropertyId::name) \
		{ \
			ApplyProperty<type>(name, prop, sheet); \
			continue; \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
	}
}

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

bool OGUI::Style::ApplyGlobalKeyword(const StyleProperty& prop)
{
	if (prop.keyword)
	{
		if (prop.value.index == (int)StyleKeyword::Initial)
		{
			ApplyInitialKeyword(prop.id);
			return true;
		}
		if (prop.value.index == (int)StyleKeyword::Unset)
		{
			ApplyUnsetKeyword(prop.id);
			return true;
		}
	}
	return false;
}

const OGUI::Style& OGUI::Style::GetInitialStyle()
{
	struct InitialStyle
	{
		InitialStyle()
		{
#define STYLEPROP(name, index, inherit, type, id, def)\
			style.name = def;
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
		}
		Style style;
	};
	static InitialStyle initStyle;
	return initStyle.style;
}

void OGUI::Style::ApplyInitialKeyword(StylePropertyId propId)
{
	auto& InitialStyle = GetInitialStyle();
#define STYLEPROP(name, ...)\
	if(propId == StylePropertyId::name) \
	{ \
		name = InitialStyle.name; \
		return; \
	}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
}

void OGUI::Style::ApplyUnsetKeyword(StylePropertyId propId)
{
	auto& InitialStyle = GetInitialStyle();
#define STYLEPROP(name, index, inherit ...)\
	if constexpr(inherit != Inherited) \
		if(propId == StylePropertyId::name) \
		{ \
			name = InitialStyle.name; \
			return; \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
}