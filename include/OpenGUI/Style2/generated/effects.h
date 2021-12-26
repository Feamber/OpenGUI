//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API StyleEffects
    {
        constexpr static std::string_view name = "effects"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t opacity = OGUI::hash("opacity"sv);
        };
        float opacity;
        void Initialize();
        static const StyleEffects& GetDefault();
        static const StyleEffects& Get(const ComputedStyle& style);
        static StyleEffects* TryGet(const ComputedStyle& style);
        static StyleEffects& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}