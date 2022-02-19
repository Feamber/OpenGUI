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

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct OGUI_API StylePosition
    {
        constexpr static ostr::string_view name = u"position"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
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
            static constexpr size_t zOrderBias = OGUI::hash(u"z-order-bias"_o);
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
        int zOrderBias;
        void Initialize();
        static const StylePosition& GetDefault();
        static const StylePosition& Get(const ComputedStyle& style);
        static StylePosition* TryGet(const ComputedStyle& style);
        static StylePosition& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, 
            const StyleMasks& override, const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, 
            const gsl::span<AnimatedProperty>& props, const StyleMasks& override);
        static RestyleDamage ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& target, 
            const gsl::span<TransitionProperty>& props, const StyleMasks& override);
        static void Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override);
        static void MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override);
        static size_t GetProperty(ostr::string_view name);
        static void SetupParser();
    };

    OGUI_API void SetStyleTransform(VisualElement* element, const gsl::span<TransformFunction>& value);
    attr("script": true)
    OGUI_API void ResetStyleTransform(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexGrow(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleFlexGrow(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexShrink(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleFlexShrink(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexBasis(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleFlexBasisPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleFlexBasisPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleFlexBasisAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleFlexBasis(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTop(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleTopPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleTopPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleTop(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleRight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleRightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleRightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleRight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBottom(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleBottomPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleBottomPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleBottom(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleLeft(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleLeftPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleLeftPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleLeft(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMarginTop(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMarginTopPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMarginTopPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMarginTop(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMarginRight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMarginRightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMarginRightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMarginRight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMarginBottom(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMarginBottomPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMarginBottomPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMarginBottom(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMarginLeft(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMarginLeftPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMarginLeftPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMarginLeft(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStylePaddingTop(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStylePaddingTopPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStylePaddingTopPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStylePaddingTop(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStylePaddingRight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStylePaddingRightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStylePaddingRightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStylePaddingRight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStylePaddingBottom(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStylePaddingBottomPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStylePaddingBottomPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStylePaddingBottom(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStylePaddingLeft(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStylePaddingLeftPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStylePaddingLeftPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStylePaddingLeft(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleWidth(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleWidthPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleWidthPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleWidthAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleHeight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleHeightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleHeightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleHeightAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleHeight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStylePosition(VisualElement* element, const YGPositionType& value);
    attr("script": true)
    OGUI_API void ResetStylePosition(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleOverflow(VisualElement* element, const EFlexOverflow& value);
    attr("script": true)
    OGUI_API void ResetStyleOverflow(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleAlignSelf(VisualElement* element, const YGAlign& value);
    attr("script": true)
    OGUI_API void ResetStyleAlignSelf(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMaxWidth(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMaxWidthPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMaxWidthPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMaxWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMaxHeight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMaxHeightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMaxHeightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleMaxHeight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMinWidth(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMinWidthPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMinWidthPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMinWidthAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleMinWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleMinHeight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleMinHeightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMinHeightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleMinHeightAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleMinHeight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexDirection(VisualElement* element, const YGFlexDirection& value);
    attr("script": true)
    OGUI_API void ResetStyleFlexDirection(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleAlignContent(VisualElement* element, const YGAlign& value);
    attr("script": true)
    OGUI_API void ResetStyleAlignContent(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleAlignItems(VisualElement* element, const YGAlign& value);
    attr("script": true)
    OGUI_API void ResetStyleAlignItems(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleJustifyContent(VisualElement* element, const YGJustify& value);
    attr("script": true)
    OGUI_API void ResetStyleJustifyContent(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexWrap(VisualElement* element, const YGWrap& value);
    attr("script": true)
    OGUI_API void ResetStyleFlexWrap(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFlexDisplay(VisualElement* element, const YGDisplay& value);
    attr("script": true)
    OGUI_API void ResetStyleFlexDisplay(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleVerticalAlign(VisualElement* element, const EInlineAlign& value);
    attr("script": true)
    OGUI_API void ResetStyleVerticalAlign(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleAspectRatio(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleAspectRatio(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleZOrderBias(VisualElement* element, const int& value);
    attr("script": true)
    OGUI_API void ResetStyleZOrderBias(VisualElement* element);
}