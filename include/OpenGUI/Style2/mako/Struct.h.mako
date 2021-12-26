//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
%for header in struct.headers:
#include "${header}"
%endfor
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API Style${struct.ident}
    {
        constexpr static std::string_view name = "${struct.name}"sv;
        constexpr static size_t hash = OGUI::hash(name);
        constexpr static bool inherited = ${str(struct.inherited).lower()};
        struct Ids
        {
        %for prop in struct.longhands:
            static constexpr size_t ${prop.ident} = OGUI::hash("${prop.name}"sv);
        %endfor
        %for prop in struct.shorthands:
            static constexpr size_t ${prop.ident} = OGUI::hash("${prop.name}"sv);
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
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}