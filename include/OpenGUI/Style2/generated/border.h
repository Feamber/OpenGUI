//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "yoga/Yoga.h"
namespace OGUI
{
    struct StyleBorder
    {
        constexpr static size_t hash = 12508467192783972168U;
        struct Id
        {
            static constexpr size_t borderLeftWidth = 13594963510097231429U;
            static constexpr size_t borderTopWidth = 3380317615930084648U;
            static constexpr size_t borderRightWidth = 10241464068792898920U;
            static constexpr size_t borderBottomWidth = 9088704890885528915U;
            static constexpr size_t borderTopLeftRadius = 8719785054287530236U;
            static constexpr size_t borderTopRightRadius = 3276279325730873300U;
            static constexpr size_t borderBottomRightRadius = 13919024445090857246U;
            static constexpr size_t borderBottomLeftRadius = 10234394799855156163U;
        };
        float borderLeftWidth;
        float borderTopWidth;
        float borderRightWidth;
        float borderBottomWidth;
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
        static void ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);
    };
}