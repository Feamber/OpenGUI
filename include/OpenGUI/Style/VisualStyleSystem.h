#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/VisualSystem.h"
#include <unordered_set>
#include <function_ref/function_ref.hpp>
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/Style.h"

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
		VisualElement* currentElement = nullptr;
	};

	struct CachedStyle
	{
		std::bitset<96> overrideMask;
		std::bitset<96> inheritMask;
		//use ptr to avoid reallocate
		Style style;
		std::vector<AnimationStyle> animStyles;
	};

	class VisualStyleSystem : public VisualSystemBase
	{
		using StyleCache = std::unordered_map<size_t, std::unique_ptr<CachedStyle>>;
		StyleCache styleCache;
		StyleMatchingContext matchingContext;
		bool _cacheInvalidated = false;

		void Traverse(VisualElement* element);

		static void FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors);
		void ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors);
		void UpdateStyle(VisualElement* element);

	public:
		OGUI_API void InvalidateCache();
		OGUI_API virtual void Update(VisualElement* Tree);
	};
}