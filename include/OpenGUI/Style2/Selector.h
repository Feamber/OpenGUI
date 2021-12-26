#pragma once
#include "OpenGUI/Configure.h"
#include <string>
#include <vector>
#include <cinttypes>
#include <optional>

namespace OGUI reflect
{
	enum class reflect attr("script":true) 
	PseudoStates : uint32_t
	{
		None = 0,
		Active = 1 << 0,
		Hover = 1 << 1,
		Checked = 1 << 2,
		Disabled = 1 << 3,
		Focus = 1 << 4,
		KeyboardFocus = 1 << 5,
		Scroll = 1 << 6,
		Root = 1 << 7,
	};
	ENUM_CLASS_FLAGS(PseudoStates)

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
		PseudoStates pseudoMask = PseudoStates::None;
		PseudoStates reversedPseudoMask = PseudoStates::None; 
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

		OGUI_API void UpdateSpecificity();
		OGUI_API bool IsSimple() { return selectors.size() == 1; }
	}; 
	class VisualElement;

	OGUI_API std::optional<StyleComplexSelector> ParseSelector(std::string_view str);
	OGUI_API bool Match(VisualElement* current, StyleComplexSelector& complexSel);
	attr("script":true)
	OGUI_API VisualElement* QueryFirst(VisualElement* root, std::string_view str);
	OGUI_API VisualElement* QueryFirst(VisualElement* root, StyleComplexSelector& complexSel);
	attr("script":true)
	OGUI_API void QueryAll(VisualElement* root, std::string_view str, std::vector<VisualElement*>& result);
	OGUI_API void QueryAll(VisualElement* root, StyleComplexSelector& complexSel, std::vector<VisualElement*>& result);
}
