#pragma once
#include <string>
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	struct AnimRunContext
	{
		float Time = 0.f;
	};

	struct AnimationStyle
	{
		bool isShared;
		StyleSheet* sheet;
		StyleKeyframes* keyframes;

		static AnimationStyle Create(bool isShared);
		static const AnimationStyle& GetInitialStyle();
		static void ApplyProperties(std::vector<OGUI::AnimationStyle>& self, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
#define	ANIMPROP(name, index, type, ...)\
		type name;
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	};
}