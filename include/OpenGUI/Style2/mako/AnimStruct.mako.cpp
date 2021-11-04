//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp

#include <memory>
#include "OpenGUI/Style2/generated/${struct.name}.h"
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
                case Id::${prop.ident}:
                    ${prop.ident} = ${prop.initial_value};
                    break;
            %endfor
            }
        }
        else
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Id::${prop.ident}:
                    ${prop.ident} = sheet.Get<${prop.type}>(prop.value);
                    break;
            %endfor
            }
        }
    }
}


bool OGUI::AnimStyle::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg, int animCount)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
    %for prop in struct.shorthands:
        case Id::${prop.ident}:
            return Parse::Parse${data.to_camel_case(prop.name)}(sheet, name, value, rule, errorMsg)
    %endfor
    }
    std::vector<std::string_view> tokens;
    std::split(value, tokens, ", ");
    //longhands
    switch(hash)
    {
    %for prop in struct.longhands:
        case Id::${prop.ident}:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse ${prop.name} value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                ${prop.type} v;
                if(${prop.parser}(tokens[i], v))
            %if prop.name=="animation-name":
                    rule.animation[i].name = v;
            %else:
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
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
    }
    return false;
}