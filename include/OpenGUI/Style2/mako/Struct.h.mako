//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako
<%! 
    from tool.style_codegen import to_camel_case
%>
#pragma once
%for header in struct.headers:
#include "${header}"
%endfor
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI reflect
{
    class VisualElement;
    using namespace ostr::literal;
    struct ${linkage} Style${struct.ident}
    {
        constexpr static ostr::string_view name = u"${struct.name}"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = ${str(struct.inherited).lower()};
        struct Ids
        {
        %for prop in struct.longhands:
            static constexpr size_t ${prop.ident} = OGUI::hash(u"${prop.name}"_o);
        %endfor
        %for prop in struct.shorthands:
            static constexpr size_t ${prop.ident} = OGUI::hash(u"${prop.name}"_o);
        %endfor
        };
    %for prop in struct.longhands:
        ${prop.storage_type} ${prop.ident};
    %endfor
        void Initialize();
        static const Style${struct.ident}& GetDefault();
        static const Style${struct.ident}& Get(const ComputedStyle& style);
        static Style${struct.ident}* TryGet(const ComputedStyle& style);
        static Style${struct.ident}& GetOrAdd(ComputedStyle& style);
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

%for prop in struct.longhands:
%if not prop.is_vector:
    attr("script": true)
%endif
    ${linkage} void SetStyle${to_camel_case(prop.name)}(VisualElement* element, ${prop.reference_type} value);
%if prop.type == "YGValue":
    attr("script": true)
    ${linkage} void SetStyle${to_camel_case(prop.name)}Pixel(VisualElement* element, float value);
    attr("script": true)
    ${linkage} void SetStyle${to_camel_case(prop.name)}Percentage(VisualElement* element, float value);
%if prop.valueRule == "Width":
    attr("script": true)
    ${linkage} void SetStyle${to_camel_case(prop.name)}Auto(VisualElement* element);
%endif
%endif
    attr("script": true)
    ${linkage} void ResetStyle${to_camel_case(prop.name)}(VisualElement* element);
%endfor
}