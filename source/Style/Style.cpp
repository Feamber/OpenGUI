#include "OpenGUI/Style/Style.h"
#include <yoga/YGNode.h>
#include <algorithm>

OGUI::Style OGUI::Style::Create(Style* parent, bool isShared)
{
	Style style = GetInitialStyle();
	style.isShared = isShared;
	if (parent)
		style.InheritData(*parent);
	return style;
}

void OGUI::Style::MergeStyle(const Style& other, std::bitset<96> mask)
{
#define STYLEPROP(name, index, inherit, ...)\
	if (mask.test((int)StylePropertyId::name)) \
	{ \
		name = other.name; \
	}
#include "OpenGUI/Style/StylePropertiesDef.h"
}

void OGUI::Style::InheritData(Style& parent)
{
#define STYLEPROP(name, index, inherit, ...)\
	if constexpr(inherit == Inherited) \
	{ \
		name = parent.name; \
	}
#include "OpenGUI/Style/StylePropertiesDef.h"
}

namespace OGUI
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
#define STYLEPROP(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
	}

	template<class T>
	void GetUnsetProperty(T& field, StylePropertyId propId)
	{
		auto& InitialStyle = Style::GetInitialStyle();
#define STYLEPROP(name, index, inherit, ...)\
	if constexpr(inherit != Inherited) \
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
	}

	template<class T>
	void GetInheritProperty(T& field, StylePropertyId propId, const Style* parent)
	{
#define STYLEPROP(name, index, inherit, ...)\
		if constexpr(inherit != Inherited) \
			if(propId == StylePropertyId::name) \
			{ \
				Assign(field, parent->name); \
			}
#include "OpenGUI/Style/StylePropertiesDef.h"
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
		assert(false);
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

void OGUI::Style::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent)
{
	for (auto& prop : props)
	{
		switch(prop.id)
		{
#define STYLEPROP(name, index, inherit, type, ...)\
		case StylePropertyId::name: \
		{ \
			if(prop.keyword) \
				GetGlobalProperty<type>(name, prop, parent); \
			else \
				GetProperty<type>(name, prop, sheet); \
			continue; \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
		}
	}
}

void OGUI::Style::ApplyPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent)
{
	//assume props is sorted
	auto iter = props.begin();
#define STYLEPROP(name, index, inherit, type, ...)\
	if(iter->id == StylePropertyId::name) \
	{ \
		if(iter->keyword) \
			GetGlobalProperty<type>(name, *iter, parent); \
		else \
			GetProperty<type>(name, *iter, sheet); \
		if (++iter == props.end()) \
			return; \
	} 
#include "OpenGUI/Style/StylePropertiesDef.h"
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
		}
		Style style;
	};
	static InitialStyle initStyle;
	return initStyle.style;
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
	return result;
}


void OGUI::Style::LerpPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent, float alpha)
{
	//assume props is sorted
	auto iter = props.begin();
#define STYLEPROP(name, index, inherit, type, ...)\
	if(iter->id == StylePropertyId::name) \
	{ \
		type value = name; \
		if(iter->keyword) \
			GetGlobalProperty<type>(value, *iter, parent); \
		else \
			GetProperty<type>(value, *iter, sheet); \
		name = Lerp(name, value, alpha); \
		if (++iter == props.end()) \
			return; \
	} 
#include "OpenGUI/Style/StylePropertiesDef.h"
}

void OGUI::Style::LerpProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent, float alpha)
{
	for (auto& prop : props)
	{
		switch (prop.id)
		{
#define STYLEPROP(name, index, inherit, type, ...)\
		case StylePropertyId::name: { \
			type value = name; \
			if(prop.keyword) \
				GetGlobalProperty<type>(value, prop, parent); \
			else \
				GetProperty<type>(value, prop, sheet); \
			name = Lerp(name, value, alpha); \
			continue; \
		}
#include "OpenGUI/Style/StylePropertiesDef.h"
		}
	}
}