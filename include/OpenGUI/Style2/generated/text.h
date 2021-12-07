//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
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
    struct OGUI_API StyleText
    {
        constexpr static std::string_view name = "text"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = true;
        struct Ids
        {
            static constexpr size_t fontSize = OGUI::hash("font-size"sv);
            static constexpr size_t color = OGUI::hash("color"sv);
        };
        float fontSize;
        Color4f color;
        void Initialize();
        static const StyleText& GetDefault();
        static const StyleText& Get(const ComputedStyle& style);
        static StyleText* TryGet(const ComputedStyle& style);
        static StyleText& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg);
    };
}