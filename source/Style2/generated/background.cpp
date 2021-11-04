//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp

#include <memory>
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"

const OGUI::StyleBackground& OGUI::StyleBackground::GetDefault()
{
    struct Helper
    {
        OGUI::StyleBackground value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleBackground& OGUI::StyleBackground::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleBackground* OGUI::StyleBackground::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::StyleBackground*)iter->second.get();
    }
}

OGUI::StyleBackground& OGUI::StyleBackground::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::StyleBackground>();
        value->Initialize();
        style.structs.insert({hash, std::static_pointer_cast<void*>(value)});
        return *value.get();
    }
    else 
    {
        return *(OGUI::StyleBackground*)iter->second.get();
    }

}

void OGUI::StyleBackground::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::StyleBackground::Initialize()
{
    backgroundColor = Color4f(1.f,1.f,1.f,1.f);
    backgroundImage = {};
}

void OGUI::StyleBackground::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(parent) : nullptr;
    OGUI::StyleBackground* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBackground>();
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
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Id::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = Color4f(1.f,1.f,1.f,1.f);
                    break;
                    }
                case Id::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = {};
                    break;
                    }
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Id::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = pst->backgroundColor;
                    break;
                    }
                case Id::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = pst->backgroundImage;
                    break;
                    }
                }
            }
        }
        else
        {
            switch(prop.id)
            {
                case Id::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Id::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = sheet.Get<std::string>(prop.value);
                    break;
                    }
            }
        }
    }
}


void OGUI::StyleBackground::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleBackground* st = TryGet(style);
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBackground>();
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
            case Id::backgroundColor:{
                auto v = fget();
                v->backgroundColor = OGUI::Lerp(sheet.Get<Color4f>(prop.from), sheet.Get<Color4f>(prop.to), prop.alpha);
                break;
                }
            case Id::backgroundImage:{
                auto v = fget();
                v->backgroundImage = OGUI::Lerp(sheet.Get<std::string>(prop.from), sheet.Get<std::string>(prop.to), prop.alpha);
                break;
                }
        }
    }
}

bool OGUI::StyleBackground::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
    }

    //longhands
    switch(hash)
    {
        case Id::backgroundColor:{
            Color4f v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse background-color value!";
                return false;
            }
            return true;
        }
        case Id::backgroundImage:{
            std::string v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse background-image value!";
                return false;
            }
            return true;
        }
    }
    return false;
}