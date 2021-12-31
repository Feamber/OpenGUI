//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Shadow.h"
#include "OpenGUI/Text/TextTypes.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "OpenGUI/Style2/Lerp/TextLerp.h"
#include "OpenGUI/Style2/Lerp/YogaLerp.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API StyleText
    {
        constexpr static std::string_view name = "text"sv;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = true;
        struct Ids
        {
            static constexpr size_t fontSize = OGUI::hash("font-size"sv);
            static constexpr size_t color = OGUI::hash("color"sv);
            static constexpr size_t fontFamily = OGUI::hash("font-family"sv);
            static constexpr size_t fontStyle = OGUI::hash("font-style"sv);
            static constexpr size_t fontWeight = OGUI::hash("font-weight"sv);
            static constexpr size_t lineHeight = OGUI::hash("line-height"sv);
            static constexpr size_t textAlign = OGUI::hash("text-align"sv);
            static constexpr size_t textShadow = OGUI::hash("text-shadow"sv);
        };
        float fontSize;
        Color4f color;
        std::vector<std::string> fontFamily;
        ETextStyle fontStyle;
        int fontWeight;
        YGValue lineHeight;
        ETextAlign textAlign;
        std::vector<TextShadow> textShadow;
        void Initialize();
        static const StyleText& GetDefault();
        static const StyleText& Get(const ComputedStyle& style);
        static StyleText* TryGet(const ComputedStyle& style);
        static StyleText& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}