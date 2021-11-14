//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Parse/Common.h"
#include "OpenGUI/Style2/Parse/Anim.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
%for header in struct.headers:
#include "${header}"
%endfor
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API AnimStyle
    {
        constexpr static size_t hash = OGUI::hash("${struct.name}"sv);
        struct Id
        {
        %for prop in struct.longhands:
            static constexpr size_t ${prop.ident} = OGUI::hash("${prop.name}"sv);
        %endfor
        };
    %for prop in struct.longhands:
        ${prop.storage_type} ${prop.ident};
    %endfor
        void Initialize();
        static void ApplyProperties(std::vector<AnimStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount);
    };
}