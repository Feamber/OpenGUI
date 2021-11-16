#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/VisualSystem.h"
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

	class VisualStyleSystem : public VisualSystemBase
	{
		StyleMatchingContext matchingContext;
		bool _cacheInvalidated = false;

		void Traverse(VisualElement* element, bool forceUpdate);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
		void ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors);
		RestyleDamage UpdateAnim(VisualElement* element);
		void UpdateStyle(VisualElement* element);

	public:
		OGUI_API void InvalidateCache();
		OGUI_API virtual void Update(VisualElement* Tree);
	};
}