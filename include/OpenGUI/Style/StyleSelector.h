#pragma once
#include <string>
#include <vector>
#include <cinttypes>
#include <optional>

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

	enum class PseudoElements : uint32_t
	{
		None,
		Before,
		After
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
			PseudoClass,
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
		PseudoElements pseudoElem = PseudoElements::None;

		void UpdateSpecificity();
		bool IsSimple() { return selectors.size() == 1; }
		void SetPseudoElement(std::string_view name);
	}; 
	class VisualElement;

	std::optional<StyleComplexSelector> ParseSelector(std::string_view str);
	bool Match(VisualElement* current, StyleComplexSelector& complexSel);
	VisualElement* QueryFirst(VisualElement* root, std::string_view str);
	VisualElement* QueryFirst(VisualElement* root, StyleComplexSelector& complexSel);
	void QueryAll(VisualElement* root, std::string_view str, std::vector<VisualElement*>& result);
	void QueryAll(VisualElement* root, StyleComplexSelector& complexSel, std::vector<VisualElement*>& result);
}
