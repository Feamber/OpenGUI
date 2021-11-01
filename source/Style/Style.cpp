#define DLL_IMPLEMENTATION
#include "OpenGUI/Style/Style.h"
#include <yoga/YGNode.h>
#include <algorithm>
#include "OpenGUI/Style/StylePropertiesDef.h"
#include "OpenGUI/Core/olog.h"

OGUI::Style OGUI::Style::Create(Style* parent)
{
	Style style = GetInitialStyle();
	if (parent)
		style.InheritData(*parent);
	return style;
}

void OGUI::Style::MergeStyle(const Style& other, std::bitset<96> mask)
{
#define GEN(name, ...)\
	if (mask.test((int)StylePropertyId::name)) \
	{ \
		name = other.name; \
	}
	STYLEPROP(GEN)
#undef GEN
}

void OGUI::Style::InheritData(Style& parent)
{
#define GEN(name, ...)\
	name = parent.name; 
	INHERITEDPROP(GEN)
#undef GEN
}

namespace OGUI::StyleTool
{
	template<class T>
	void Assign(T& field, const T& value)
	{
		field = value;
	}
	template<class T, class Y>
	void Assign(T& field, const Y& value)
	{ }
	template<class T>
	void GetInitialProperty(T& field, StylePropertyId propId)
	{
		auto& InitialStyle = Style::GetInitialStyle();
#define GEN(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
		STYLEPROP(GEN)
#undef GEN
	}

	template<class T>
	void GetUnsetProperty(T& field, StylePropertyId propId)
	{
		auto& InitialStyle = Style::GetInitialStyle();
#define GEN(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
		NONINHERITEDPROP(GEN)
#undef GEN
	}

	template<class T>
	void GetInheritProperty(T& field, StylePropertyId propId, const Style* parent)
	{
#define GEN(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, parent->name); \
		}
		INHERITEDPROP(GEN)
#undef GEN
	}

	template<class T>
	void GetGlobalProperty(T& field, const StyleProperty& prop, const Style* parent)
	{
		if (prop.value.index == (int)StyleKeyword::Initial)
		{
			GetInitialProperty<T>(field, prop.id);
			return;
		}
		if (prop.value.index == (int)StyleKeyword::Unset)
		{
			GetUnsetProperty<T>(field, prop.id);
			return;
		}
		if (prop.value.index == (int)StyleKeyword::Inherit && parent)
		{
			GetInheritProperty<T>(field, prop.id, parent);
			return;
		}
		OUNREACHABLE
	}

	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, void>
		GetProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
	{
		field = sheet.Get<T>(prop.value);
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, void>
		GetProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
	{
		field = (T)sheet.Get<int>(prop.value);
	}

}

void OGUI::Style::ApplyPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent)
{
	for (auto& prop : props)
	{
		switch(prop.id)
		{
#define GEN(name, type, ...)\
		case StylePropertyId::name: \
		{ \
			if(prop.keyword) \
				StyleTool::GetGlobalProperty<type>(name, prop, parent); \
			else \
				StyleTool::GetProperty<type>(name, prop, sheet); \
			continue; \
		}
		STYLEPROP(GEN)
#undef GEN
		default: 
			return;
		}
	}
}

const OGUI::Style& OGUI::Style::GetInitialStyle()
{
	struct InitialStyle
	{
		InitialStyle()
		{
#define GEN(name, type, id, def, ...)\
			style.name = def;
			STYLEPROP(GEN)
#undef GEN
		}
		Style style;
	};
	static InitialStyle initStyle;
	return initStyle.style;
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

namespace OGUI
{
	template<class T>
	T LerpF(const T& a, const T& b, float alpha)
	{
		return a * (1 - alpha) + b * alpha;
	}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, T>
		Lerp(const T& a, const T& b, float alpha)
	{
		return LerpF(a, b, alpha);
	}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>, T>
		Lerp(const T& a, const T& b, float alpha)
	{
		return b;
	}
	Vector2f Lerp(const Vector2f& a, const Vector2f& b, float alpha)
	{
		return LerpF(a, b, alpha);
	}
	Color4f Lerp(const Color4f& a, const Color4f& b, float alpha)
	{
		return LerpF(a, b, alpha);
	}

	YGValue Lerp(const YGValue& a, const YGValue& b, float alpha)
	{
		OASSERT(a.unit == b.unit);
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
#define GEN(name, ...) \
	result.name = Lerp(a.name, b.name, alpha);
	STYLEPROP(GEN)
#undef GEN
	return result;
}


void OGUI::Style::LerpPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent, float alpha)
{
	for (auto& prop : props)
	{
		switch (prop.id)
		{
#define GEN(name, type, ...)\
		case StylePropertyId::name: { \
			type value = name; \
			if(prop.keyword) \
				StyleTool::GetGlobalProperty<type>(value, prop, parent); \
			else \
				StyleTool::GetProperty<type>(value, prop, sheet); \
			name = Lerp(name, value, alpha); \
			continue; \
		}
		STYLEPROP(GEN)
#undef GEN
		default:
			return;
		}
	}
}