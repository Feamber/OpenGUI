#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style/StyleSelector.h"

void OGUI::VisualStyleSystem::Traverse(VisualElement* element, int depth)
{
	const auto& ess = element->GetStyleSheets();
	for (auto& ss : ess)
		matchingContext.styleSheetStack.push_back(ss);
	int originCustomPropCount = element->_style.GetCustomPropCount();
	{
		matchingContext.currentElement = element;
		std::vector<SelectorMatchRecord> result;
		FindMatches(matchingContext, result);
		ProcessMatchedRules(element, result);
	}
	element->Traverse([this](VisualElement* element, int depth) { Traverse(element, depth) }, depth);
}

void OGUI::VisualStyleSystem::Update(VisualElement* Tree)
{

}
namespace OGUI
{
	MatchResult Match(VisualElement* element, StyleSelector& selector)
	{
		MatchResult result;
		bool match = true;

		switch (selector.type)
		{
			case StyleSelector::Wildcard:
			case StyleSelector::PseudoClass:
				break;
			case StyleSelector::Class:
				match = element->ContainClass(selector.value);
				break;
			case StyleSelector::Name:
				match = element->name == selector.value;
				break;
			case StyleSelector::Type:
				match = element->IsA(selector.value);
				break;
			default:
				match = false;
				//assert(false);
		}
		if (match)
		{
			if (selector.pseudoMask != 0)
			{
				match = (selector.pseudoMask & element->_pseudoMask) == selector.pseudoMask;
				if (match)
					result.dependencyPseudoMask = selector.pseudoMask;
				else
					result.triggerPseudoMask = selector.pseudoMask;
			}
			if (selector.reversedPseudoMask != 0)
			{
				match &= (selector.pseudoMask & ~element->_pseudoMask) == selector.pseudoMask;
				if (match)
					result.dependencyPseudoMask |= selector.pseudoMask;
				else
					result.triggerPseudoMask |= selector.pseudoMask;
			}
		}
		result.success = match;
		return result;
	}

	bool Match(StyleMatchingContext& context, StyleComplexSelector* complexSel)
	{
		//https://speakerdeck.com/constellation/css-jit-just-in-time-compiled-css-selectors-in-webkit
		auto current = context.currentElement;
		auto& selectors = complexSel->selectors;
		int count = selectors.size();
		int index = count - 1;
		VisualElement* saved = nullptr;
		int savedIndex = -1;

		while (index >= 0)
		{
			if (current == nullptr)
				break;

			MatchResult result = Match(current, selectors[index]);
			context.processor(current, result);
			if (!result.success)
			{
				if (index < count - 1 && selectors[index + 1].relationship == StyleSelectorRelationship::Descendent)
				{
					current = current->GetParent();
					continue;
				}
				if (saved != nullptr)
				{
					current = saved;
					index = savedIndex;
					continue;
				}
				break;
			}
			if (index < count - 1 && selectors[index + 1].relationship == StyleSelectorRelationship::Descendent)
			{
				saved = current->GetParent();
				savedIndex = index;
			}
			if (--index < 0)
				return true;

			current = current->GetParent();
		}
		return false;
	}

	void Lookup(
		StyleMatchingContext& context,
		std::vector<SelectorMatchRecord>& matchedSelectors,
		StyleSheet::SelectorMap& map,
		std::string_view input,
		SelectorMatchRecord& record)
	{
		auto range = map.equal_range(input);
		if (range.first != map.end())
		{
			for (auto iter = range.first; iter != range.second; ++iter)
			{
				if (Match(context, iter->second))
				{
					record.complexSelector = iter->second;
					matchedSelectors.push_back(record);
				}
			}
		}
	}
}

void OGUI::VisualStyleSystem::FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors)
{
	matchedSelectors.clear();
	//check(context.currentElement != nullptr);
	auto element = context.currentElement;
	auto sheetCount = context.styleSheetStack.size();
	for (int i = 0; i < sheetCount; ++i)
	{
		StyleSheet* sheet = context.styleSheetStack[i];
		SelectorMatchRecord record{sheet, i};
		Lookup(context, matchedSelectors, sheet->typeSelectors, element->GetTypeName(), record);
		Lookup(context, matchedSelectors, sheet->typeSelectors, "*", record);
		if(!element->name.empty())
			Lookup(context, matchedSelectors, sheet->nameSelectors, element->name, record);
		for(auto& cls : element->classes)
			Lookup(context, matchedSelectors, sheet->classSelectors, element->classes, record);
	}
}
