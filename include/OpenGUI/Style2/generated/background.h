//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct OGUI_API StyleBackground
    {
        constexpr static ostr::string_view name = u"background"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t backgroundColor = OGUI::hash(u"background-color"_o);
            static constexpr size_t backgroundImage = OGUI::hash(u"background-image"_o);
            static constexpr size_t backgroundMaterial = OGUI::hash(u"background-material"_o);
        };
        Color4f backgroundColor;
        ostr::string backgroundImage;
        ostr::string backgroundMaterial;
        void Initialize();
        static const StyleBackground& GetDefault();
        static const StyleBackground& Get(const ComputedStyle& style);
        static StyleBackground* TryGet(const ComputedStyle& style);
        static StyleBackground& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, 
            const StyleMasks& override, const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, 
            const gsl::span<AnimatedProperty>& props, const StyleMasks& override);
        static RestyleDamage ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& src, const ComputedStyle& dst, 
            const gsl::span<TransitionProperty>& props, const StyleMasks& override);
        static void Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override);
        static void MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override);
        static size_t GetProperty(ostr::string_view name);
        static void SetupParser();
    };

    attr("script": true)
    OGUI_API void SetStyleBackgroundColor(VisualElement* element, const Color4f& value);
    attr("script": true)
    OGUI_API void ResetStyleBackgroundColor(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBackgroundImage(VisualElement* element, const ostr::string_view& value);
    attr("script": true)
    OGUI_API void ResetStyleBackgroundImage(VisualElement* element);
    attr("script": true)
    OGUI_API void SetStyleBackgroundMaterial(VisualElement* element, const ostr::string_view& value);
    attr("script": true)
    OGUI_API void ResetStyleBackgroundMaterial(VisualElement* element);
}