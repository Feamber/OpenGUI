#pragma once
#include <unordered_map>
#include <string>
#include <gsl/span>
#include <OpenGUI/Style/VariantStorage.h>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/StylePropertiesDef.h"
namespace OGUI
{
	struct AnimationStyle;
	struct AnimRunContext;
	struct Style
	{
		bool isShared;

		static const Style& GetInitialStyle();
		static Style Create(Style* parent, bool isShared);
		void MergeStyle(const Style& other, std::bitset<96> mask);
		void ApplyPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent);
		void ApplyAnimation(const AnimationStyle& anim, const AnimRunContext& ctx, const Style* parent);
		void LerpPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
			const Style* parent, float alpha);
		void InheritData(Style& parent);
#define	GEN(name, type, ...)\
		type name;
		STYLEPROP(GEN)
#undef  GEN
	};

	Style Lerp(const Style& a, const Style& b, float alpha);
}