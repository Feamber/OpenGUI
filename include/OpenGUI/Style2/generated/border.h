//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "OpenGUI/Style2/Lerp/YogaLerp.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API StyleBorder
    {
        constexpr static std::string_view name = "border"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t borderTopWidth = OGUI::hash("border-top-width"sv);
            static constexpr size_t borderRightWidth = OGUI::hash("border-right-width"sv);
            static constexpr size_t borderBottomWidth = OGUI::hash("border-bottom-width"sv);
            static constexpr size_t borderLeftWidth = OGUI::hash("border-left-width"sv);
            static constexpr size_t borderTopLeftRadius = OGUI::hash("border-top-left-radius"sv);
            static constexpr size_t borderTopRightRadius = OGUI::hash("border-top-right-radius"sv);
            static constexpr size_t borderBottomRightRadius = OGUI::hash("border-bottom-right-radius"sv);
            static constexpr size_t borderBottomLeftRadius = OGUI::hash("border-bottom-left-radius"sv);
            static constexpr size_t borderRadius = OGUI::hash("border-radius"sv);
        };
        float borderTopWidth;
        float borderRightWidth;
        float borderBottomWidth;
        float borderLeftWidth;
        YGValue borderTopLeftRadius;
        YGValue borderTopRightRadius;
        YGValue borderBottomRightRadius;
        YGValue borderBottomLeftRadius;
        void Initialize();
        static const StyleBorder& GetDefault();
        static const StyleBorder& Get(const ComputedStyle& style);
        static StyleBorder* TryGet(const ComputedStyle& style);
        static StyleBorder& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}