#include "OpenGUI/Style/StyleSelector.h"
#include <array>
#include <string_view>
#include <bitset>

void OGUI::StyleSelector::AddPseudoClass(std::string_view pseudoClass)
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
		{"inactive", PseudoStates::Active, true},
		{"enabled", PseudoStates::Disabled, true}
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

template<class T>
int bitcount(const T& value)
{
	std::bitset<sizeof(T)*8> bitset(value);
	return (int)bitset.count();
}

void OGUI::StyleComplexSelector::UpdateSpecificity()
{
	constexpr int typeWeight = 1, classWeight = 10, nameWeight = 100;
	specificity = 0;
	for (auto& sel : selectors)
	{
		for (auto& part : sel.parts)
		{
			switch (part.type)
			{
				case StyleSelector::Type:
					specificity += typeWeight;
					break;
				case StyleSelector::Class:
					specificity += classWeight;
					break;
				case StyleSelector::Name:
					specificity += nameWeight;
					break;
				default:
					break;
			}
		}
		specificity += bitcount(sel.pseudoMask) * classWeight;
		specificity += bitcount(sel.reversedPseudoMask) * classWeight;
	}
}
