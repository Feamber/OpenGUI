//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct SAMPLECONTROLS_API StyleSample
    {
        constexpr static ostr::string_view name = u"sample"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t someValue = OGUI::hash(u"some-value"_o);
        };
        float someValue;
        void Initialize();
        static const StyleSample& GetDefault();
        static const StyleSample& Get(const ComputedStyle& style);
        static StyleSample* TryGet(const ComputedStyle& style);
        static StyleSample& GetOrAdd(ComputedStyle& style);
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
    SAMPLECONTROLS_API void SetStyleSomeValue(VisualElement* element, const float& value);
    attr("script": true)
    SAMPLECONTROLS_API void ResetStyleSomeValue(VisualElement* element);
}