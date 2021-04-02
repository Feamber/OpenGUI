#define DLL_IMPLEMENTATION
#include "OpenGUI/Style/StyleProperty.h"
#include <unordered_map>

OGUI::StylePropertyId OGUI::PropertyNameToId(std::string_view name)
{
	static std::unordered_map<std::string_view, StylePropertyId> name2id =
	{
#define GEN(id, type, name, ...) \
		{name, StylePropertyId::id},
		STYLEPROP(GEN)
		ANIMPROP(GEN)
#undef GEN
	};
	auto iter = name2id.find(name);
	if (iter != name2id.end())
		return iter->second;
	else
		return StylePropertyId::_End;
}

int OGUI::PropertyNameToOrder(std::string_view name)
{
	static std::unordered_map<std::string_view, StylePropertyId> name2id =
	{
#define GEN(id, type, name, ...) \
		{name, StylePropertyId::id},
		STYLEPROP(GEN)
		ANIMPROP(GEN)
#undef GEN
		{"transform", StylePropertyId::translation},
		{"margin", StylePropertyId::marginLeft},
		{"border-width", StylePropertyId::paddingLeft},
		{"padding", StylePropertyId::borderLeftWidth},
		{"border-radius", StylePropertyId::borderTopLeftRadius},
	};
	auto iter = name2id.find(name);
	if (iter != name2id.end())
		return (int)iter->second;
	else
		return (int)StylePropertyId::_End;
}

std::string_view OGUI::PropertyIdToName(StylePropertyId id)
{
	static constexpr std::string_view id2name[] =
	{
#define GEN(id, type, name, ...) \
		name,
		STYLEPROP(GEN)
		ANIMPROP(GEN)
#undef GEN
	};
	if (id < StylePropertyId::_End)
		return id2name[(uint8_t)id];
	else
		return "unknown";
}

std::bitset<96> OGUI::GetInheritMask()
{
	std::bitset<96> res;
#define GEN(id, _1, _2, _3, inherit) \
	if constexpr(inherit == Inheritance::Inherited) \
		res.set((int)StylePropertyId::id); 
		STYLEPROP(GEN)
#undef GEN
	return res;
}
