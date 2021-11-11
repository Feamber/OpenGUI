//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp
#define DLL_IMPLEMENTATION
#include <memory>
#include "OpenGUI/Style2/generated/border.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "border_shorthands.h"

const OGUI::StyleBorder& OGUI::StyleBorder::GetDefault()
{
    struct Helper
    {
        OGUI::StyleBorder value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleBorder& OGUI::StyleBorder::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleBorder* OGUI::StyleBorder::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::StyleBorder*)iter->second.ptr.get();
    }
}

OGUI::StyleBorder& OGUI::StyleBorder::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::StyleBorder>();
        value->Initialize();
        style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
        return *value.get();
    }
    else 
    {
        return *(OGUI::StyleBorder*)iter->second.ptr.get();
    }

}

void OGUI::StyleBorder::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::StyleBorder::Initialize()
{
    borderLeftWidth = 0.f;
    borderTopWidth = 0.f;
    borderRightWidth = 0.f;
    borderBottomWidth = 0.f;
    borderTopLeftRadius = YGValueZero;
    borderTopRightRadius = YGValueZero;
    borderBottomRightRadius = YGValueZero;
    borderBottomLeftRadius = YGValueZero;
}

void OGUI::StyleBorder::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleBorder* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StyleBorder*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBorder>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBorder>(*st);
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
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Id::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = 0.f;
                    break;
                    }
                case Id::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = 0.f;
                    break;
                    }
                case Id::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = 0.f;
                    break;
                    }
                case Id::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = 0.f;
                    break;
                    }
                case Id::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = YGValueZero;
                    break;
                    }
                case Id::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = YGValueZero;
                    break;
                    }
                case Id::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = YGValueZero;
                    break;
                    }
                case Id::borderBottomLeftRadius:{
                    auto v = fget();
                    v->borderBottomLeftRadius = YGValueZero;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Id::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = pst->borderLeftWidth;
                    break;
                    }
                case Id::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = pst->borderTopWidth;
                    break;
                    }
                case Id::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = pst->borderRightWidth;
                    break;
                    }
                case Id::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = pst->borderBottomWidth;
                    break;
                    }
                case Id::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = pst->borderTopLeftRadius;
                    break;
                    }
                case Id::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = pst->borderTopRightRadius;
                    break;
                    }
                case Id::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = pst->borderBottomRightRadius;
                    break;
                    }
                case Id::borderBottomLeftRadius:{
                    auto v = fget();
                    v->borderBottomLeftRadius = pst->borderBottomLeftRadius;
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
                case Id::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::borderBottomLeftRadius:{
                    auto v = fget();
                    v->borderBottomLeftRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


void OGUI::StyleBorder::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleBorder* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StyleBorder*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBorder>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBorder>(*st);
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
            case Id::borderLeftWidth:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderLeftWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderLeftWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderLeftWidth = OGUI::Lerp(v->borderLeftWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderLeftWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::borderTopWidth:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderTopWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderTopWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderTopWidth = OGUI::Lerp(v->borderTopWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderTopWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::borderRightWidth:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderRightWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderRightWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderRightWidth = OGUI::Lerp(v->borderRightWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderRightWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::borderBottomWidth:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderBottomWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderBottomWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderBottomWidth = OGUI::Lerp(v->borderBottomWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderBottomWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::borderTopLeftRadius:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderTopLeftRadius = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderTopLeftRadius = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->borderTopLeftRadius = OGUI::Lerp(v->borderTopLeftRadius, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->borderTopLeftRadius = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::borderTopRightRadius:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderTopRightRadius = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderTopRightRadius = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->borderTopRightRadius = OGUI::Lerp(v->borderTopRightRadius, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->borderTopRightRadius = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::borderBottomRightRadius:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderBottomRightRadius = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderBottomRightRadius = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->borderBottomRightRadius = OGUI::Lerp(v->borderBottomRightRadius, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->borderBottomRightRadius = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::borderBottomLeftRadius:{
                auto v = fget();
                if(prop.alpha == 0.f)
                    v->borderBottomLeftRadius = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderBottomLeftRadius = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->borderBottomLeftRadius = OGUI::Lerp(v->borderBottomLeftRadius, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->borderBottomLeftRadius = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            default: break;
        }
    }
}

bool OGUI::StyleBorder::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
        case Id::borderRadius:
            return Parse::ParseBorderRadius(sheet, name, value, rule, errorMsg);
        default: break;
    }
    StyleKeyword keyword = StyleKeyword::None;
    ParseValue(value, keyword);
    if(keyword != StyleKeyword::None)
    {
        switch(hash)
        {
            case Id::borderLeftWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderTopWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderRightWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderBottomWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderTopLeftRadius:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderTopRightRadius:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderBottomRightRadius:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::borderBottomLeftRadius:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            default: break;
        }
        return false;
    }
    //longhands
    switch(hash)
    {
        case Id::borderLeftWidth:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-left-width value!";
                return false;
            }
            return true;
        }
        case Id::borderTopWidth:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-top-width value!";
                return false;
            }
            return true;
        }
        case Id::borderRightWidth:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-right-width value!";
                return false;
            }
            return true;
        }
        case Id::borderBottomWidth:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-bottom-width value!";
                return false;
            }
            return true;
        }
        case Id::borderTopLeftRadius:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-top-left-radius value!";
                return false;
            }
            return true;
        }
        case Id::borderTopRightRadius:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-top-right-radius value!";
                return false;
            }
            return true;
        }
        case Id::borderBottomRightRadius:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-bottom-right-radius value!";
                return false;
            }
            return true;
        }
        case Id::borderBottomLeftRadius:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push(v)});
            else
            {
                errorMsg = "failed to parse border-bottom-left-radius value!";
                return false;
            }
            return true;
        }
        default: break;
    }
    return false;
}