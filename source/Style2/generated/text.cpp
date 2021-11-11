//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp
#define DLL_IMPLEMENTATION
#include <memory>
#include "OpenGUI/Style2/generated/text.h"
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
        return (OGUI::StyleText*)iter->second.ptr.get();
    }
}

OGUI::StyleText& OGUI::StyleText::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::StyleText>();
        value->Initialize();
        style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
        return *value.get();
    }
    else 
    {
        return *(OGUI::StyleText*)iter->second.ptr.get();
    }

}

void OGUI::StyleText::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::StyleText::Initialize()
{
    fontSize = 20.f;
    color = Color4f(0,0,0,1);
}

void OGUI::StyleText::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleText* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StyleText*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleText>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleText>(*st);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
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
                case Id::color:{
                    auto v = fget();
                    v->color = Color4f(0,0,0,1);
                    break;
                    }
                default: break;
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
                case Id::color:{
                    auto v = fget();
                    v->color = pst->color;
                    break;
                    }
                default: break;
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
                case Id::color:{
                    auto v = fget();
                    v->color = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


void OGUI::StyleText::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleText* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StyleText*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleText>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleText>(*st);
            style.structs.erase(iter);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
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
                if(prop.alpha == 0.f)
                    v->fontSize = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontSize = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->fontSize = OGUI::Lerp(v->fontSize, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->fontSize = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::color:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->color = sheet.Get<Color4f>(prop.from);
                else if(prop.alpha == 1.f)
                    v->color = sheet.Get<Color4f>(prop.to);
                else if(prop.from == prop.to)
                    v->color = OGUI::Lerp(v->color, sheet.Get<Color4f>(prop.to), prop.alpha);
                else
                    v->color = OGUI::Lerp(sheet.Get<Color4f>(prop.from), sheet.Get<Color4f>(prop.to), prop.alpha);
                break;
                }
            default: break;
        }
    }
}

bool OGUI::StyleText::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    StyleKeyword keyword = StyleKeyword::None;
    ParseValue(value, keyword);
    if(keyword != StyleKeyword::None)
    {
        switch(hash)
        {
            case Id::fontSize:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::color:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            default: break;
        }
        return false;
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
        case Id::color:{
            Color4f v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse color value!";
                return false;
            }
            return true;
        }
        default: break;
    }
    return false;
}