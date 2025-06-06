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

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct OGUI_API StyleText
    {
        constexpr static ostr::string_view name = u"text"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = true;
        struct Ids
        {
            static constexpr size_t fontSize = OGUI::hash(u"font-size"_o);
            static constexpr size_t color = OGUI::hash(u"color"_o);
            static constexpr size_t fontFamily = OGUI::hash(u"font-family"_o);
            static constexpr size_t fontStyle = OGUI::hash(u"font-style"_o);
            static constexpr size_t fontWeight = OGUI::hash(u"font-weight"_o);
            static constexpr size_t lineHeight = OGUI::hash(u"line-height"_o);
            static constexpr size_t textAlign = OGUI::hash(u"text-align"_o);
            static constexpr size_t letterSpacing = OGUI::hash(u"letter-spacing"_o);
            static constexpr size_t wordSpacing = OGUI::hash(u"word-spacing"_o);
            static constexpr size_t textShadow = OGUI::hash(u"text-shadow"_o);
            static constexpr size_t textDecorationColor = OGUI::hash(u"text-decoration-color"_o);
            static constexpr size_t textDecorationLine = OGUI::hash(u"text-decoration-line"_o);
            static constexpr size_t textDecorationThickness = OGUI::hash(u"text-decoration-thickness"_o);
            static constexpr size_t textJustify = OGUI::hash(u"text-justify"_o);
        };
        float fontSize;
        Color4f color;
        std::vector<ostr::string> fontFamily;
        ETextStyle fontStyle;
        int fontWeight;
        YGValue lineHeight;
        ETextAlign textAlign;
        float letterSpacing;
        float wordSpacing;
        std::vector<TextShadow> textShadow;
        Color4f textDecorationColor;
        ETextDecorationLine textDecorationLine;
        float textDecorationThickness;
        ETextJustify textJustify;
        void Initialize();
        static const StyleText& GetDefault();
        static const StyleText& Get(const ComputedStyle& style);
        static StyleText* TryGet(const ComputedStyle& style);
        static StyleText& GetOrAdd(ComputedStyle& style);
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
    OGUI_API void SetStyleFontSize(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleFontSize(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleColor(VisualElement* element, const Color4f& value);
    attr("script": true)
    OGUI_API void ResetStyleColor(VisualElement* element);
    OGUI_API void SetStyleFontFamily(VisualElement* element, const gsl::span<const ostr::string>& value);
    attr("script": true)
    OGUI_API void ResetStyleFontFamily(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFontStyle(VisualElement* element, const ETextStyle& value);
    attr("script": true)
    OGUI_API void ResetStyleFontStyle(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleFontWeight(VisualElement* element, const int& value);
    attr("script": true)
    OGUI_API void ResetStyleFontWeight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleLineHeight(VisualElement* element, const YGValue& value);
    attr("script": true)
    OGUI_API void SetStyleLineHeightPixel(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleLineHeightPercentage(VisualElement* element, float value);
    attr("script": true)
    OGUI_API void SetStyleLineHeightAuto(VisualElement* element);
    attr("script": true)
    OGUI_API void ResetStyleLineHeight(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTextAlign(VisualElement* element, const ETextAlign& value);
    attr("script": true)
    OGUI_API void ResetStyleTextAlign(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleLetterSpacing(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleLetterSpacing(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleWordSpacing(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleWordSpacing(VisualElement* element);
    OGUI_API void SetStyleTextShadow(VisualElement* element, const gsl::span<const TextShadow>& value);
    attr("script": true)
    OGUI_API void ResetStyleTextShadow(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTextDecorationColor(VisualElement* element, const Color4f& value);
    attr("script": true)
    OGUI_API void ResetStyleTextDecorationColor(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTextDecorationLine(VisualElement* element, const ETextDecorationLine& value);
    attr("script": true)
    OGUI_API void ResetStyleTextDecorationLine(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTextDecorationThickness(VisualElement* element, const float& value);
    attr("script": true)
    OGUI_API void ResetStyleTextDecorationThickness(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleTextJustify(VisualElement* element, const ETextJustify& value);
    attr("script": true)
    OGUI_API void ResetStyleTextJustify(VisualElement* element);
}