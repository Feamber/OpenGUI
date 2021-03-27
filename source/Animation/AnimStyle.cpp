#include "OpenGUI/Animation/AnimStyle.h"
#include <algorithm>

OGUI::AnimationStyle OGUI::AnimationStyle::Create(bool isShared)
{
	AnimationStyle style = GetInitialStyle();
	style.isShared = isShared;
	return style;
}

const OGUI::AnimationStyle& OGUI::AnimationStyle::GetInitialStyle()
{
	struct InitialStyle
	{
		InitialStyle()
		{
#define ANIMPROP(name, index, type, id, def)\
			style.name = def;
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
		}
		AnimationStyle style;
	};
	static InitialStyle initStyle;
	return initStyle.style;
}

namespace OGUI
{

template<class T>
std::enable_if_t<!std::is_enum_v<T>, void>
	ApplyProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
{
	field = sheet.Get<T>(prop.value);
}

template<class T>
std::enable_if_t<std::is_enum_v<T>, void>
	ApplyProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
{
	field = (T)sheet.Get<int>(prop.value);
}

}

void OGUI::AnimationStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const AnimationStyle* parent)
{
	for (auto& prop : props)
	{
		if (ApplyGlobalKeyword(prop, parent))
			continue;
#define ANIMPROP(name, index, type, ...)\
		if(prop.id == StylePropertyId::name) \
		{ \
			ApplyProperty<type>(name, prop, sheet); \
			continue; \
		}
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	}
}

bool OGUI::AnimationStyle::ApplyGlobalKeyword(const StyleProperty& prop, const AnimationStyle* parent)
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
		//TODO: should we support animation property inheritance?
		if (prop.value.index == (int)StyleKeyword::Inherit && parent)
		{
			ApplyInheritKeyword(prop.id, parent);
			return true;
		}
	}
	return false;
}

void OGUI::AnimationStyle::ApplyInitialKeyword(StylePropertyId propId)
{
	auto& InitialStyle = GetInitialStyle();
#define ANIMPROP(name, ...)\
	if(propId == StylePropertyId::name) \
	{ \
		name = InitialStyle.name; \
		return; \
	}
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
}

void OGUI::AnimationStyle::ApplyUnsetKeyword(StylePropertyId propId)
{
	//anim style is noninherited by default
	ApplyInitialKeyword(propId);
}

void OGUI::AnimationStyle::ApplyInheritKeyword(StylePropertyId propId, const AnimationStyle * parent)
{
#define ANIMPROP(name, ...)\
	if(propId == StylePropertyId::name) \
	{ \
		name = parent->name; \
		return; \
	}
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
}
