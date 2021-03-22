#pragma once
#include "OpenGUI/VisualSystem.h"
#include <unordered_set>
#include <function_ref/function_ref.hpp>
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	struct SelectorMatchRecord
	{
		StyleSheet* sheet;
		int sheetIndex;
		StyleComplexSelector* complexSelector;
		int Compare(const SelectorMatchRecord& other);
	};
	struct StyleMatchingContext
	{
		std::vector<StyleSheet*> styleSheetStack;
		VisualElement* currentElement;
	};

	struct Style;
	class VisualStyleSystem : public VisualSystemBase
	{
		std::unordered_map<size_t, std::unique_ptr<Style>> styleCache;

		void Traverse(VisualElement* element, int depth);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
		static void ApplyMatchedRules(VisualElement* element, std::vector<SelectorMatchRecord>& matchedSelectors, std::unordered_map<size_t, std::unique_ptr<Style>>& styleCache);

	public:
		virtual void Update(VisualElement* Tree);
	};
}