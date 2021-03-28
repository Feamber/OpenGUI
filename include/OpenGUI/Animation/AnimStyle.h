#pragma once
#include <string>
#include "OpenGUI/Style/StyleSheet.h"

namespace OGUI
{
	struct AnimRunContext
	{
		float time = 0.f;
		bool Goingback = false;
		bool Yielding = false;
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
		bool operator==(const AnimationStyle& other);
#define	ANIMPROP(name, index, type, ...)\
		type name;
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	};
}