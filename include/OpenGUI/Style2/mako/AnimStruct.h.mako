//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
%for header in struct.headers:
#include "${header}"
%endfor
namespace OGUI
{
    using namespace ostr::literal;
    struct OGUI_API AnimStyle
    {
        struct Ids
        {
        %for prop in struct.longhands:
            static constexpr size_t ${prop.ident} = OGUI::hash(u"${prop.name}"_o);
        %endfor
        };
    %for prop in struct.longhands:
        ${prop.storage_type} ${prop.ident};
    %endfor
        void Initialize();
        static void ApplyProperties(std::vector<AnimStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static void SetupParser();
    };
}