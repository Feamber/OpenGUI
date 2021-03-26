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
		enum Kind
		{
			Wildcard,
			Class,
			Name,
			Type,
		};
		struct Part
		{
			Kind type;
			std::string value;
		};
		std::vector<Part> parts;
		uint32_t pseudoMask = 0;
		uint32_t reversedPseudoMask = 0; 
		StyleSelectorRelationship relationship = StyleSelectorRelationship::None;

		void AddPseudoClass(std::string_view name);
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
