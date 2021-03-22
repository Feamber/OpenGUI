#include "OpenGUI/Style/Style.h"
#include <yoga/YGNode.h>

OGUI::Style OGUI::Style::Create(Style* parent, bool isShared)
{
	Style style = GetInitialStyle();
	style.isShared = isShared;
	if (parent)
		style.InheritData(*parent);
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
#define STYLEPROP(name, index, inherit, ...)\
	if constexpr(inherit != Inherited) \
		if(propId == StylePropertyId::name) \
		{ \
			name = InitialStyle.name; \
			return; \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
}

namespace OGUI
{
	constexpr size_t GetInheritedDataSize()
	{
		size_t size = 0;
#define STYLEPROP(name, index, inherit, type, ...)\
	if constexpr(inherit == Inherited) \
		size+=sizeof(type);
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
		return size;
	}
}

template<class T>
void WriteTo(char*& ptr, const T& value)
{
	memcpy(ptr, &value, sizeof(T));
	ptr += sizeof(T);
}

size_t HashBuffer(const char* p, size_t s)
{
	size_t result = 14695981039346656037ULL;
	const size_t prime = 31;
	for (size_t i = 0; i < s; ++i)
	{
		result = p[i] + (result * prime);
	}
	return result;
}

size_t OGUI::Style::GetInheritedHash()
{
	constexpr size_t size = GetInheritedDataSize();
	char buffer[size];
	char* ptr = buffer;
#define STYLEPROP(name, index, inherit, type, ...)\
	if constexpr(inherit == Inherited) \
		WriteTo(ptr, name);
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
	return HashBuffer(buffer, size);
}