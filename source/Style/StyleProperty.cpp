#include "OpenGUI/Style/StyleProperty.h"
#include <unordered_map>

OGUI::StylePropertyId OGUI::PropertyNameToId(std::string_view name)
{
	static std::unordered_map<std::string_view, StylePropertyId> name2id =
	{
#define STYLEPROP(id, index, inherit, type, name, ...) \
		{name, StylePropertyId::id},
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef STYLEPROP
	};
	auto iter = name2id.find(name);
	if (iter != name2id.end())
		return iter->second;
	else
		return StylePropertyId::Num;
}

std::string_view OGUI::PropertyIdToName(StylePropertyId id)
{
	static constexpr std::string_view id2name[] =
	{
#define STYLEPROP(id, index, inherit, type, name, ...) \
		name,
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef STYLEPROP
	};
	if (id < StylePropertyId::Num)
		return id2name[(uint8_t)id];
	else
		return "unknown";
}
