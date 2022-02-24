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

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct OGUI_API StyleBorder
    {
        constexpr static ostr::string_view name = u"border"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
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
            const StyleMasks& override, const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, 
            const gsl::span<AnimatedProperty>& props, const StyleMasks& override);
        static RestyleDamage ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& src, const ComputedStyle& dst, 
            const gsl::span<TransitionProperty>& props);
        static void Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override);
        static void MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override);
        static size_t GetProperty(ostr::string_view name);
        static void SetupParser();
    };

    attr("script": true)
    OGUI_API void SetStyleBorderTopWidth(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleBorderTopWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderRightWidth(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleBorderRightWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomWidth(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleBorderBottomWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderLeftWidth(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleBorderLeftWidth(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderTopLeftRadius(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleBorderTopLeftRadiusPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleBorderTopLeftRadiusPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleBorderTopLeftRadius(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderTopRightRadius(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleBorderTopRightRadiusPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleBorderTopRightRadiusPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleBorderTopRightRadius(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomRightRadius(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomRightRadiusPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomRightRadiusPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleBorderBottomRightRadius(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomLeftRadius(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomLeftRadiusPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleBorderBottomLeftRadiusPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void ResetStyleBorderBottomLeftRadius(VisualElement* element);
}