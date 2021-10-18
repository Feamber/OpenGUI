#pragma once
#include "OpenGUI/Style2/Properties.h"
<% from data import struct %>
namespace OGUI
{
    struct Style${struct.ident}
    {
        constexpr static size_t hash = ${struct.hash};
        %for prop in struct.longhands:
            ${prop.type} ${prop.ident};
        %endfor
        static const Style${struct.ident}& GetDefault();
        static const Style${struct.ident}& Get(Style& style);
        static const Style${struct.ident}* TryGet(Style& style);
        static Style${struct.ident}& GetOrAdd(Style& style);
        static void Dispose(Style& style);
        static void Merge(Style& style, const Style& other, gsl::span<StyleProperty>& props);
        static void Initialize(Style& style);
        static void ApplySS(Style& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const Style& parent);
    };
}