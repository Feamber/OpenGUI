#pragma once
#include <unordered_map>
#include <string>
#include <gsl/span>
#include <OpenGUI/Style/VariantStorage.h>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/StyleSheet.h"
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
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent);
		void ApplyPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent);
		void ApplyAnimation(const AnimationStyle& anim, const AnimRunContext& ctx, const Style* parent);
		void LerpPropertiesFast(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
			const Style* parent, float alpha);
		void LerpProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, 
			const Style* parent, float alpha);
		void InheritData(Style& parent);
#define	STYLEPROP(name, index, inherit, type, ...)\
		type name;
#include "OpenGUI/Style/StylePropertiesDef.h"
	};

	Style Lerp(const Style& a, const Style& b, float alpha);
}