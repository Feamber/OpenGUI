//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp

#include <memory>
#include "OpenGUI/Style2/generated/${struct.name}.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
%if struct.shorthands:
#include "${struct.name}_shorthands.h"
%endif

const OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetDefault()
{
    struct Helper
    {
        OGUI::Style${struct.ident} value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::Style${struct.ident}* OGUI::Style${struct.ident}::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::Style${struct.ident}*)iter->second.get();
    }
}

OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::Style${struct.ident}>();
        value->Initialize();
        style.structs.insert({hash, std::static_pointer_cast<void*>(value)});
        return *value.get();
    }
    else 
    {
        return *(OGUI::Style${struct.ident}*)iter->second.get();
    }

}

void OGUI::Style${struct.ident}::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::Style${struct.ident}::Initialize()
{
%for prop in struct.longhands:
    ${prop.ident} = ${prop.initial_value};
%endfor
}

void OGUI::Style${struct.ident}::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(parent) : nullptr;
    OGUI::Style${struct.ident}* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>();
            value->Initialize();
            style.structs.insert({hash, std::static_pointer_cast<void*>(value)});
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
        %if not struct.inherited:
            || prop.value.index == (int)StyleKeyword::Unset
        %endif
             )
            {
                switch(prop.id)
                {
            %for prop in struct.longhands:
                case Id::${prop.ident}:{
                    auto v = fget();
                    v->${prop.ident} = ${prop.initial_value};
                    break;
                    }
            %endfor
                }
            }
            else
            { 
                switch(prop.id)
                {
            %for prop in struct.longhands:
                case Id::${prop.ident}:{
                    auto v = fget();
                    v->${prop.ident} = pst->${prop.ident};
                    break;
                    }
            %endfor
                }
            }
        }
        else
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Id::${prop.ident}:{
                    auto v = fget();
                    v->${prop.ident} = sheet.Get<${prop.type}>(prop.value);
                    break;
                    }
            %endfor
            }
        }
    }
}


void OGUI::Style${struct.ident}::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::Style${struct.ident}* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>();
            value->Initialize();
            style.structs.insert({hash, std::static_pointer_cast<void*>(value)});
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
        %for prop in struct.longhands:
            case Id::${prop.ident}:{
                auto v = fget();
                v->${prop.ident} = OGUI::Lerp(sheet.Get<${prop.type}>(prop.from), sheet.Get<${prop.type}>(prop.to), prop.alpha);
                break;
                }
        %endfor
        }
    }
}

bool OGUI::Style${struct.ident}::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
    %for prop in struct.shorthands:
        case Id::${prop.ident}:
            return Parse::Parse${data.to_camel_case(prop.name)}(sheet, name, value, rule, errorMsg);
    %endfor
    }

    //longhands
    switch(hash)
    {
    %for prop in struct.longhands:
        case Id::${prop.ident}:{
            ${prop.type} v;
            if(${prop.parser}(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse ${prop.name} value!";
                return false;
            }
            return true;
        }
    %endfor
    }
    return false;
}