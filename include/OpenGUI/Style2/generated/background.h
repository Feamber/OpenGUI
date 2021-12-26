//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API StyleBackground
    {
        constexpr static std::string_view name = "background"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t backgroundColor = OGUI::hash("background-color"sv);
            static constexpr size_t backgroundImage = OGUI::hash("background-image"sv);
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
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}