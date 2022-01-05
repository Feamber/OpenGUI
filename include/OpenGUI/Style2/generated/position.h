//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/Transform.h"
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
    struct OGUI_API StylePosition
    {
        constexpr static ostr::string_view name = u"position"_o;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t transform = OGUI::hash(u"transform"_o);
            static constexpr size_t flexGrow = OGUI::hash(u"flex-grow"_o);
            static constexpr size_t flexShrink = OGUI::hash(u"flex-shrink"_o);
            static constexpr size_t flexBasis = OGUI::hash(u"flex-basis"_o);
            static constexpr size_t top = OGUI::hash(u"top"_o);
            static constexpr size_t right = OGUI::hash(u"right"_o);
            static constexpr size_t bottom = OGUI::hash(u"bottom"_o);
            static constexpr size_t left = OGUI::hash(u"left"_o);
            static constexpr size_t marginTop = OGUI::hash(u"margin-top"_o);
            static constexpr size_t marginRight = OGUI::hash(u"margin-right"_o);
            static constexpr size_t marginBottom = OGUI::hash(u"margin-bottom"_o);
            static constexpr size_t marginLeft = OGUI::hash(u"margin-left"_o);
            static constexpr size_t paddingTop = OGUI::hash(u"padding-top"_o);
            static constexpr size_t paddingRight = OGUI::hash(u"padding-right"_o);
            static constexpr size_t paddingBottom = OGUI::hash(u"padding-bottom"_o);
            static constexpr size_t paddingLeft = OGUI::hash(u"padding-left"_o);
            static constexpr size_t width = OGUI::hash(u"width"_o);
            static constexpr size_t height = OGUI::hash(u"height"_o);
            static constexpr size_t position = OGUI::hash(u"position"_o);
            static constexpr size_t overflow = OGUI::hash(u"overflow"_o);
            static constexpr size_t alignSelf = OGUI::hash(u"align-self"_o);
            static constexpr size_t maxWidth = OGUI::hash(u"max-width"_o);
            static constexpr size_t maxHeight = OGUI::hash(u"max-height"_o);
            static constexpr size_t minWidth = OGUI::hash(u"min-width"_o);
            static constexpr size_t minHeight = OGUI::hash(u"min-height"_o);
            static constexpr size_t flexDirection = OGUI::hash(u"flex-direction"_o);
            static constexpr size_t alignContent = OGUI::hash(u"align-content"_o);
            static constexpr size_t alignItems = OGUI::hash(u"align-items"_o);
            static constexpr size_t justifyContent = OGUI::hash(u"justify-content"_o);
            static constexpr size_t flexWrap = OGUI::hash(u"flex-wrap"_o);
            static constexpr size_t flexDisplay = OGUI::hash(u"flex-display"_o);
            static constexpr size_t verticalAlign = OGUI::hash(u"vertical-align"_o);
            static constexpr size_t aspectRatio = OGUI::hash(u"aspect-ratio"_o);
            static constexpr size_t margin = OGUI::hash(u"margin"_o);
            static constexpr size_t padding = OGUI::hash(u"padding"_o);
        };
        std::vector<TransformFunction> transform;
        float flexGrow;
        float flexShrink;
        YGValue flexBasis;
        YGValue top;
        YGValue right;
        YGValue bottom;
        YGValue left;
        YGValue marginTop;
        YGValue marginRight;
        YGValue marginBottom;
        YGValue marginLeft;
        YGValue paddingTop;
        YGValue paddingRight;
        YGValue paddingBottom;
        YGValue paddingLeft;
        YGValue width;
        YGValue height;
        YGPositionType position;
        EFlexOverflow overflow;
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
        EInlineAlign verticalAlign;
        float aspectRatio;
        void Initialize();
        static const StylePosition& GetDefault();
        static const StylePosition& Get(const ComputedStyle& style);
        static StylePosition* TryGet(const ComputedStyle& style);
        static StylePosition& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}