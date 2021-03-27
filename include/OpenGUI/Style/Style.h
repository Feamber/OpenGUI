#pragma once
#include <unordered_map>
#include <string>
#include <gsl/span>
#include <OpenGUI/Style/VariantStorage.h>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleSheet.h"
namespace OGUI
{
	struct Style
	{
		bool isShared;

		static const Style& GetInitialStyle();
		static Style Create(Style* parent, bool isShared);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent);
		bool ApplyGlobalKeyword(const StyleProperty& props, const Style* parent);
		void ApplyCustomProperties(const StyleSheetStorage& sheet, const gsl::span<CustomStyleProperty>& props);
		void ApplyInitialKeyword(StylePropertyId propId);
		void ApplyUnsetKeyword(StylePropertyId propId);
		void ApplyInheritKeyword(StylePropertyId propId, const Style* parent);
		void InheritData(Style& parent);
#define	STYLEPROP(name, index, inherit, type, ...)\
		type name;
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
	};

	Style Lerp(const Style& a, const Style& b, float alpha);
}