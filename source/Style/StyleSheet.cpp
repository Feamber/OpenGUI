#include "OpenGUI/Style/StyleSheet.h"

void OGUI::StyleSheet::Initialize()
{
	int i = 0;
	for (auto& complexSel : styleSelectors)
	{
		complexSel.priority = i;
		auto& lastSel = complexSel.selectors.back();
		SelectorMap* mapPtr;
		std::string key = lastSel.value;
		switch (lastSel.type)
		{
			case StyleSelector::Class:
				mapPtr = &classSelectors;
				break;
			case StyleSelector::Name:
				mapPtr = &nameSelectors;
				break;
			case StyleSelector::Type:
			case StyleSelector::Wildcard:
				if (lastSel.value.empty())
					key = "*";
				mapPtr = &typeSelectors;
				break;
			case StyleSelector::PseudoClass:
				key = "*";
				mapPtr = &typeSelectors;
				break;
			default:
				//assert(false);
				break;
		}
		// !![HANG FOR BUILD]!!
		//mapPtr->insert(key, &complexSel);
		++i;
	}
}
