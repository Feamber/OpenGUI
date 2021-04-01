#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
#include "OpenGUI/Core/olog.h"

void OGUI::StyleSheet::Initialize()
{
	for (auto&& [i, complexSel] : ipair(styleSelectors))
	{
		complexSel.priority = i;
		auto& lastSel = complexSel.selectors.back();
		SelectorMap* mapPtr = nullptr;
		std::string_view key;
		if (lastSel.parts.size() == 0)
		{
			key = "*";
			mapPtr = &typeSelectors;
		}
		else
		{
			key = lastSel.parts.back().value;
			switch (lastSel.parts.back().type)
			{
				case StyleSelector::Class:
					mapPtr = &classSelectors;
					break;
				case StyleSelector::Name:
					mapPtr = &nameSelectors;
					break;
				case StyleSelector::Type:
				case StyleSelector::Wildcard:
					if (lastSel.parts.back().value.empty())
						key = "*";
					mapPtr = &typeSelectors;
					break;
				default:
					OUNREACHABLE
					break;
			}
		}
		
		mapPtr->insert(std::make_pair(key, i));
	}
	for (auto&& [i, keyframes] : ipair(styleKeyframes))
		namedKeyframes.insert({keyframes.name, i});
}