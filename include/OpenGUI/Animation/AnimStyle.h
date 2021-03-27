#pragma once
#include <string>
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	struct AnimationStyle
	{
		bool isShared;

		static AnimationStyle Create(bool isShared);
		static const AnimationStyle& GetInitialStyle();
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const AnimationStyle* parent);
		bool ApplyGlobalKeyword(const StyleProperty& props, const AnimationStyle* parent);
		void ApplyInitialKeyword(StylePropertyId propId);
		void ApplyUnsetKeyword(StylePropertyId propId);
		void ApplyInheritKeyword(StylePropertyId propId, const AnimationStyle* parent);

#define	ANIMPROP(name, index, type, ...)\
		type name;
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	};
}