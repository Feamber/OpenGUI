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
    using namespace ostr::literal;
    struct OGUI_API StyleBorder
    {
        constexpr static ostr::string_view name = u"border"_o;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t borderTopWidth = OGUI::hash(u"border-top-width"_o);
            static constexpr size_t borderRightWidth = OGUI::hash(u"border-right-width"_o);
            static constexpr size_t borderBottomWidth = OGUI::hash(u"border-bottom-width"_o);
            static constexpr size_t borderLeftWidth = OGUI::hash(u"border-left-width"_o);
            static constexpr size_t borderTopLeftRadius = OGUI::hash(u"border-top-left-radius"_o);
            static constexpr size_t borderTopRightRadius = OGUI::hash(u"border-top-right-radius"_o);
            static constexpr size_t borderBottomRightRadius = OGUI::hash(u"border-bottom-right-radius"_o);
            static constexpr size_t borderBottomLeftRadius = OGUI::hash(u"border-bottom-left-radius"_o);
            static constexpr size_t borderRadius = OGUI::hash(u"border-radius"_o);
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