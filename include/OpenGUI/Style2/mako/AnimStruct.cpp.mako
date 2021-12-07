//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako
<%! 
    from tool.style_codegen import to_small_camel_case, to_camel_case
%>

#include <memory>
#include "${struct.include_path}"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Rule.h"

void OGUI::AnimStyle::Initialize()
{
%for prop in struct.longhands:
    ${prop.ident} = ${prop.initial_value};
%endfor
}

void OGUI::AnimStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:
                    ${prop.ident} = ${prop.initial_value};
                    break;
            %endfor
                default: break;
            }
        }
        else
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:
                    ${prop.ident} = sheet.Get<${prop.view_type}>(prop.value);
                    break;
            %endfor
                default: break;
            }
        }
    }
}


bool OGUI::AnimStyle::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    %if struct.shorthands:
    switch(hash)
    {
    %for prop in struct.shorthands:
        case Ids::${prop.ident}:
            return Parse::Parse${to_camel_case(prop.name)}(sheet, name, value, rule, errorMsg)
    %endfor
        default: break;
    }
    %endif
    std::vector<std::string_view> tokens;
    std::split(value, tokens, ", ");
    //longhands
    switch(hash)
    {
    %for prop in struct.longhands:
        case Ids::${prop.ident}:{
            int count = std::min((int)tokens.size(), animCount);
            for(int i=0; i<count; ++i)
            {
                ${prop.storage_type} v;
                if(${prop.parser}(tokens[i], v))
            %if prop.name=="animation-name":
                    rule.animation[i].name = v;
            %else:
                {
                    auto handle = sheet.Push(v);
                    for(int j=i; j<animCount; j+=count)
                        rule.animation[j].properties.push_back({hash, handle});
                }
            %endif
                else
                {
                    errorMsg = "failed to parse ${prop.name} value!";
                    return false;
                }
            }
            return true;
        }
    %endfor
        default: break;
    }
    return false;
}