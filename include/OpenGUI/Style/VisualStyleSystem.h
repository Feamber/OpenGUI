#pragma once
#include "OpenGUI/VisualSystem.h"
#include <unordered_set>
#include <function_ref/function_ref.hpp>
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	struct MatchResult
	{
		bool success;
		uint32_t triggerPseudoMask;
		uint32_t dependencyPseudoMask;
	};
	struct SelectorMatchRecord
	{
		StyleSheet* sheet;
		int sheetIndex;
		StyleComplexSelector* complexSelector;
		int Compare(const SelectorMatchRecord& other);
	};
	struct StyleMatchingContext
	{
		using ProcessFunction = tl::function_ref<void(VisualElement*, const MatchResult&)>;
		StyleMatchingContext(ProcessFunction pf)
			: processor(pf) {}
		std::vector<StyleSheet*> styleSheetStack;
		VisualElement* currentElement;
		ProcessFunction processor;
	};

	struct Style;
	class VisualStyleSystem : public VisualSystemBase
	{
		std::unordered_map<size_t, std::unique_ptr<Style>> styleCache;

		void Traverse(VisualElement* element, int depth);
		virtual void Update(VisualElement* Tree);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
		static void ApplyMatchedRules(VisualElement* element, std::vector<SelectorMatchRecord>& matchedSelectors, std::unordered_map<size_t, std::unique_ptr<Style>>& styleCache);
	};
}