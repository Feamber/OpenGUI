//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/Common.h"
#include "OpenGUI/Style2/Parse/Common.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "yoga/Yoga.h"
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/Parse/Math.h"
#include "OpenGUI/Style2/Parse/Yoga.h"
#include "OpenGUI/Style2/Lerp/Math.h"
#include "OpenGUI/Style2/Lerp/Yoga.h"
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API StylePosition
    {
        constexpr static std::string_view name = "position"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Id
        {
            static constexpr size_t transform = OGUI::hash("transform"sv);
            static constexpr size_t flexGrow = OGUI::hash("flex-grow"sv);
            static constexpr size_t flexShrink = OGUI::hash("flex-shrink"sv);
            static constexpr size_t flexBasis = OGUI::hash("flex-basis"sv);
            static constexpr size_t left = OGUI::hash("left"sv);
            static constexpr size_t top = OGUI::hash("top"sv);
            static constexpr size_t right = OGUI::hash("right"sv);
            static constexpr size_t bottom = OGUI::hash("bottom"sv);
            static constexpr size_t marginLeft = OGUI::hash("margin-left"sv);
            static constexpr size_t marginTop = OGUI::hash("margin-top"sv);
            static constexpr size_t marginRight = OGUI::hash("margin-right"sv);
            static constexpr size_t marginBottom = OGUI::hash("margin-bottom"sv);
            static constexpr size_t paddingLeft = OGUI::hash("padding-left"sv);
            static constexpr size_t paddingTop = OGUI::hash("padding-top"sv);
            static constexpr size_t paddingRight = OGUI::hash("padding-right"sv);
            static constexpr size_t paddingBottom = OGUI::hash("padding-bottom"sv);
            static constexpr size_t width = OGUI::hash("width"sv);
            static constexpr size_t height = OGUI::hash("height"sv);
            static constexpr size_t position = OGUI::hash("position"sv);
            static constexpr size_t overflow = OGUI::hash("overflow"sv);
            static constexpr size_t alignSelf = OGUI::hash("align-self"sv);
            static constexpr size_t maxWidth = OGUI::hash("max-width"sv);
            static constexpr size_t maxHeight = OGUI::hash("max-height"sv);
            static constexpr size_t minWidth = OGUI::hash("min-width"sv);
            static constexpr size_t minHeight = OGUI::hash("min-height"sv);
            static constexpr size_t flexDirection = OGUI::hash("flex-direction"sv);
            static constexpr size_t alignContent = OGUI::hash("align-content"sv);
            static constexpr size_t alignItems = OGUI::hash("align-items"sv);
            static constexpr size_t justifyContent = OGUI::hash("justify-content"sv);
            static constexpr size_t flexWrap = OGUI::hash("flex-wrap"sv);
            static constexpr size_t flexDisplay = OGUI::hash("flex-display"sv);
            static constexpr size_t margin = OGUI::hash("margin"sv);
        };
        std::vector<TransformFunction> transform;
        float flexGrow;
        float flexShrink;
        YGValue flexBasis;
        YGValue left;
        YGValue top;
        YGValue right;
        YGValue bottom;
        YGValue marginLeft;
        YGValue marginTop;
        YGValue marginRight;
        YGValue marginBottom;
        YGValue paddingLeft;
        YGValue paddingTop;
        YGValue paddingRight;
        YGValue paddingBottom;
        YGValue width;
        YGValue height;
        YGPositionType position;
        YGOverflow overflow;
        YGAlign alignSelf;
        YGValue maxWidth;
        YGValue maxHeight;
        YGValue minWidth;
        YGValue minHeight;
        YGFlexDirection flexDirection;
        YGAlign alignContent;
        YGAlign alignItems;
        YGJustify justifyContent;
        YGWrap flexWrap;
        YGDisplay flexDisplay;
        void Initialize();
        static const StylePosition& GetDefault();
        static const StylePosition& Get(const ComputedStyle& style);
        static StylePosition* TryGet(const ComputedStyle& style);
        static StylePosition& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg);
    };
}