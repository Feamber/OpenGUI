
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/olog.h"
#include <bitset>

void OGUI::StyleSelector::AddPseudoClass(ostr::string_view name)
{
	PseudoStates state = PseudoStates::Active;
	bool reverse = false;
	switchstr(name)
	{
		casestr("active") state = PseudoStates::Active; break;
		casestr("hover") state = PseudoStates::Hover; break;
		casestr("checked") state = PseudoStates::Checked; break;
		casestr("disabled") state = PseudoStates::Disabled; break;
		casestr("focus") state = PseudoStates::Focus; break;
		casestr("keyboardFocus") state = PseudoStates::KeyboardFocus; break;
		casestr("scroll") state = PseudoStates::Scroll; break;
		casestr("root") state = PseudoStates::Root; break;
		casestr("inactive") state = PseudoStates::Active; reverse = true; break;
		casestr("enabled") state = PseudoStates::Disabled;  reverse = true; break;
		default:
			OUNREACHABLE
			return;
	}
	if (!reverse)
		pseudoMask |= state;
	else
		reversedPseudoMask |= state;
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
		specificity += bitcount((uint32_t)sel.pseudoMask) * classWeight;
		specificity += bitcount((uint32_t)sel.reversedPseudoMask) * classWeight;
	}
}