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
		StyleSheet sheet;
		int sheetIndex;
		StyleComplexSelector* complexSelector;
	};
	struct StyleMatchingContext
	{
		std::vector<StyleSheet*> styleSheetStack;
		VisualElement* currentElement;
		tl::function_ref<void(VisualElement*, MatchResult)> processor;
	};

	class VisualStyleSystem : public VisualSystemBase
	{
		StyleMatchingContext matchingContext;
		void Traverse(VisualElement* element, int depth);
		virtual void Update(VisualElement* Tree);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
	};
}