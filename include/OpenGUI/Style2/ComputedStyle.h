#pragma once
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include <gsl/span>
#include <unordered_map>
namespace OGUI
{
    struct ComputedStyle;

    struct StyleDesc
    {
		std::string_view name;
		void (*Dispose)(ComputedStyle& style);
        void (*Initialize)(ComputedStyle& style);
        void (*ApplyProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent);
		void (*ApplyAnimatedProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
		bool (*ParseProperties)(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);
    };

	struct StyleRegistry
	{
		std::vector<StyleDesc> descriptions;
		static const StyleRegistry& Get();
	};

	void RegisterStyleStruct(const StyleDesc& Registry);

	struct ComputedStyle
	{
		std::unordered_map<size_t, std::shared_ptr<void*>> structs;
		static const ComputedStyle& GetInitialStyle();
		static ComputedStyle Create();
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent);
        void ApplyAnimatedProperties(const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
	};

}