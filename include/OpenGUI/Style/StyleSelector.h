#pragma once
#include <string>
#include <vector>
#include <cinttypes>

namespace OGUI
{
	enum class PseudoStates : uint32_t
	{
		Active = 1 << 0,
		Hover = 1 << 1,
		Checked = 1 << 2,
		Disabled = 1 << 3,
		Focus = 1 << 4,
		Root = 1 << 5,
	};

	enum class StyleSelectorRelationship : uint8_t
	{
		None,
		Child,
		Descendent
	};

	struct StyleSelector
	{
		enum
		{
			Wildcard,
			Type,
			Class,
			Name,
			PseudoClass
		} type;
		std::string value;
		uint32_t pseudoMask = 0;
		uint32_t reversedPseudoMask = 0; 
		StyleSelectorRelationship relationship;

		void AddPseudoClass(const char* name);
	};


	struct StyleComplexSelector
	{
		std::vector<StyleSelector> selectors;
		int ruleIndex;
		int priority;
		int specificity;

		void UpdateSpecificity();
		bool IsSimple() { return selectors.size() == 1; }
	};
}
