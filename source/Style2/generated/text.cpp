//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp

#include <memory>
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"

const OGUI::StyleText& OGUI::StyleText::GetDefault()
{
    struct Helper
    {
        OGUI::StyleText value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleText& OGUI::StyleText::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleText* OGUI::StyleText::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::StyleText*)iter->second.get();
    }
}

OGUI::StyleText& OGUI::StyleText::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::StyleText>();
        value->Initialize();
        style.structs.insert({hash, std::static_pointer_cast<void*>(value)});
        return *value.get();
    }
    else 
    {
        return *(OGUI::StyleText*)iter->second.get();
    }

}

void OGUI::StyleText::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::StyleText::Initialize()
{
    fontSize = 20.f;
}

void OGUI::StyleText::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(parent) : nullptr;
    OGUI::StyleText* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleText>();
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
             )
            {
                switch(prop.id)
                {
                case Id::fontSize:{
                    auto v = fget();
                    v->fontSize = 20.f;
                    break;
                    }
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Id::fontSize:{
                    auto v = fget();
                    v->fontSize = pst->fontSize;
                    break;
                    }
                }
            }
        }
        else
        {
            switch(prop.id)
            {
                case Id::fontSize:{
                    auto v = fget();
                    v->fontSize = sheet.Get<float>(prop.value);
                    break;
                    }
            }
        }
    }
}


void OGUI::StyleText::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleText* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleText>();
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
            case Id::fontSize:{
                auto v = fget();
                v->fontSize = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
        }
    }
}

bool OGUI::StyleText::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
    }

    //longhands
    switch(hash)
    {
        case Id::fontSize:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse font-size value!";
                return false;
            }
            return true;
        }
    }
    return false;
}