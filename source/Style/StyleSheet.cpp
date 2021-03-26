#include "OpenGUI/Style/StyleSheet.h"

void OGUI::StyleSheet::Initialize()
{
	int i = 0;
	for (auto& complexSel : styleSelectors)
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
					assert(false);
					break;
			}
		}
		
		mapPtr->insert(std::make_pair(key, i));
		++i;
	}
}