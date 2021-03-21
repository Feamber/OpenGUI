#pragma once
#include <unordered_map>
#include <string>
#include <gsl/span>
#include <OpenGUI/Style/VariantStorage.h>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleRule.h"
#include "OpenGUI/Style/StyleSheet.h"
namespace OGUI
{
	struct StyleSheet;
	struct Style
	{
		bool isShared;
		struct CustomPropGroup
		{
			StyleSheet* sheet;
			std::vector<CustomStyleProperty> props;
		};
		std::vector<CustomPropGroup> customProps;

		static const Style& GetInitialStyle();
		
		int GetCustomPropCount() { return customProps.size(); }
		static Style Create(Style* parent, bool isShared);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		bool ApplyGlobalKeyword(const StyleProperty& props);
		void ApplyCustomProperties(const StyleSheetStorage& sheet, const gsl::span<CustomStyleProperty>& props);
		void ApplyInitialKeyword(StylePropertyId propId);
		void ApplyUnsetKeyword(StylePropertyId propId);
		void InheritData(Style& parent);
		size_t GetInheritedHash();
#define	STYLEPROP(name, index, inherit, type, ...)\
		type name;
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef	STYLEPROP
	};

}