#pragma once
#include "OpenGUI/Configure.h"
#include <unordered_set>
#include <function_ref/function_ref.hpp>
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Properties.h"

namespace OGUI
{
	struct SelectorMatchRecord
	{
		StyleSheet* sheet;
		int sheetIndex;
		StyleComplexSelector* complexSelector;
	};
	struct StyleMatchingContext
	{
		std::vector<StyleSheet*> styleSheetStack;
		VisualElement* currentElement = nullptr;
	};

	class VisualStyleSystem
	{
		StyleMatchingContext matchingContext;

		void Traverse(VisualElement* element, bool forceUpdate, bool refresh);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
		void ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors, bool refresh);
		RestyleDamage UpdateAnim(VisualElement* element);
		void UpdateStyle(VisualElement* element, const std::vector<StyleSheet*>& ss);

	public:
		OGUI_API void Update(VisualElement* Tree, bool refresh);
	};
}