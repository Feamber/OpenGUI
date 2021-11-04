//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Core/Math.h"
namespace OGUI
{
    struct StyleBackground
    {
        constexpr static size_t hash = 8018830855182204725U;
        struct Id
        {
            static constexpr size_t backgroundColor = 6146824217856065527U;
            static constexpr size_t backgroundImage = 5857326841327635783U;
        };
        Color4f backgroundColor;
        std::string backgroundImage;
        void Initialize();
        static const StyleBackground& GetDefault();
        static const StyleBackground& Get(const ComputedStyle& style);
        static StyleBackground* TryGet(const ComputedStyle& style);
        static StyleBackground& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static void ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);
    };
}