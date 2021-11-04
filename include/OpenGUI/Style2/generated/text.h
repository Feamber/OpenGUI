//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
namespace OGUI
{
    struct StyleText
    {
        constexpr static size_t hash = 5501995993259654724U;
        struct Id
        {
            static constexpr size_t fontSize = 5562663152763689101U;
        };
        float fontSize;
        void Initialize();
        static const StyleText& GetDefault();
        static const StyleText& Get(const ComputedStyle& style);
        static StyleText* TryGet(const ComputedStyle& style);
        static StyleText& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static void ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);
    };
}