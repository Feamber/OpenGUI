#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include <gsl/span>
#include <memory>
#include <unordered_map>
namespace OGUI
{
    struct ComputedStyle;

    struct StyleDesc
    {
		std::string_view name;
		size_t hash;
		bool inherited;
        void (*ApplyProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent);
		RestyleDamage (*ApplyAnimatedProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
    };

	struct StyleRegistry
	{
		std::vector<StyleDesc> descriptions;
		static const StyleRegistry& Get();
	};

	OGUI_API void RegisterStyleStruct(const StyleDesc& Registry);
	
	OGUI_API void RegisterBuiltinStructs();

	struct ComputedStyle
	{
		struct ComputedStruct
		{
			std::shared_ptr<void> ptr;
			bool owned = true;
		};
		std::unordered_map<size_t, ComputedStruct> structs;
		ComputedStyle() = default;
		ComputedStyle(const ComputedStyle& other);
		ComputedStyle(ComputedStyle&& other) = default;
		ComputedStyle& operator=(const ComputedStyle& other);
		ComputedStyle& operator=(ComputedStyle&& other) = default;
		static ComputedStyle Create(const ComputedStyle* parent);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent);
        RestyleDamage ApplyAnimatedProperties(const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
	};

}