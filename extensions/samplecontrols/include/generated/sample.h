//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Style2/Parse/CommonParse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse/MathParse.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct SAMPLECONTROLS_API StyleSample
    {
        constexpr static std::string_view name = "sample"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t someValue = OGUI::hash("some-value"sv);
        };
        float someValue;
        void Initialize();
        static const StyleSample& GetDefault();
        static const StyleSample& Get(const ComputedStyle& style);
        static StyleSample* TryGet(const ComputedStyle& style);
        static StyleSample& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg);
    };
}