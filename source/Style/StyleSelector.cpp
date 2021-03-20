#include "OpenGUI/Style/StyleSelector.h"
#include <array>
#include <string_view>

void OGUI::StyleSelector::AddPseudoClass(const char* pseudoClass)
{
	struct Name2State
	{
		std::string_view name;
		PseudoStates state;
		bool reverse;
	};
	static const Name2State map[] =
	{
		{"active", PseudoStates::Active, false},
		{"hover", PseudoStates::Hover, false},
		{"checked", PseudoStates::Checked, false},
		{"disabled", PseudoStates::Disabled, false},
		{"focus", PseudoStates::Focus, false},
		{"root", PseudoStates::Root, false},
		{"active", PseudoStates::Active, true},
		{"disabled", PseudoStates::Disabled, true}
	};
	bool founded = false;
	for (auto& state : map)
	{
		if (state.name == pseudoClass)
		{
			if (!state.reverse)
				pseudoMask |= (uint32_t)state.state;
			else
				reversedPseudoMask |= (uint32_t)state.state;
			founded = true;
			break;
		}
	}

	if(!founded)
	{
		//TODO: log unknown pseudo class
	}
}
