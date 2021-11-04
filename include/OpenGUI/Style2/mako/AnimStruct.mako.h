//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
%for header in struct.headers:
#include "${header}"
%endfor
namespace OGUI
{
    struct AnimStyle
    {
        constexpr static size_t hash = ${struct.hash}U;
        //Resolved reference
		StyleSheet* sheet;
		StyleKeyframes* keyframes;
        struct Id
        {
        %for prop in struct.longhands:
            static constexpr size_t ${prop.ident} = ${prop.hash}U;
        %endfor
        };
    %for prop in struct.longhands:
        ${prop.type} ${prop.ident};
    %endfor
        void Initialize();
        static void ApplyProperties(std::vector<AnimStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg, int animCount);
    };
}