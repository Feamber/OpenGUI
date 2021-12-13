//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


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
    fontFamily = {};
    fontStyle = TextStyle::Normal;
    fontWeight = 400;
    lineHeight = 1.f;
    textAlign = TextAlign::Start;
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
                case Ids::fontSize:{
                    auto v = fget();
                    v->fontSize = 20.f;
                    break;
                    }
                case Ids::color:{
                    auto v = fget();
                    v->color = Color4f(0,0,0,1);
                    break;
                    }
                case Ids::fontFamily:{
                    auto v = fget();
                    v->fontFamily = {};
                    break;
                    }
                case Ids::fontStyle:{
                    auto v = fget();
                    v->fontStyle = TextStyle::Normal;
                    break;
                    }
                case Ids::fontWeight:{
                    auto v = fget();
                    v->fontWeight = 400;
                    break;
                    }
                case Ids::lineHeight:{
                    auto v = fget();
                    v->lineHeight = 1.f;
                    break;
                    }
                case Ids::textAlign:{
                    auto v = fget();
                    v->textAlign = TextAlign::Start;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::fontSize:{
                    auto v = fget();
                    v->fontSize = pst->fontSize;
                    break;
                    }
                case Ids::color:{
                    auto v = fget();
                    v->color = pst->color;
                    break;
                    }
                case Ids::fontFamily:{
                    auto v = fget();
                    v->fontFamily = pst->fontFamily;
                    break;
                    }
                case Ids::fontStyle:{
                    auto v = fget();
                    v->fontStyle = pst->fontStyle;
                    break;
                    }
                case Ids::fontWeight:{
                    auto v = fget();
                    v->fontWeight = pst->fontWeight;
                    break;
                    }
                case Ids::lineHeight:{
                    auto v = fget();
                    v->lineHeight = pst->lineHeight;
                    break;
                    }
                case Ids::textAlign:{
                    auto v = fget();
                    v->textAlign = pst->textAlign;
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
                case Ids::fontSize:{
                    auto v = fget();
                    v->fontSize = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::color:{
                    auto v = fget();
                    v->color = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Ids::fontFamily:{
                    auto v = fget();
                    v->fontFamily = sheet.Get<std::string>(prop.value);
                    break;
                    }
                case Ids::fontStyle:{
                    auto v = fget();
                    v->fontStyle = sheet.Get<TextStyle>(prop.value);
                    break;
                    }
                case Ids::fontWeight:{
                    auto v = fget();
                    v->fontWeight = sheet.Get<int>(prop.value);
                    break;
                    }
                case Ids::lineHeight:{
                    auto v = fget();
                    v->lineHeight = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::textAlign:{
                    auto v = fget();
                    v->textAlign = sheet.Get<TextAlign>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleText::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleText* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
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
            case Ids::fontSize:{
                auto v = fget();
                auto prevValue = v->fontSize;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->fontSize = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontSize = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->fontSize = OGUI::Lerp(v->fontSize, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->fontSize = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->fontSize)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::color:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
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
            case Ids::fontFamily:{
                auto v = fget();
                auto prevValue = v->fontFamily;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->fontFamily = sheet.Get<std::string>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontFamily = sheet.Get<std::string>(prop.to);
                else if(prop.from == prop.to)
                    v->fontFamily = OGUI::Lerp(v->fontFamily, sheet.Get<std::string>(prop.to), prop.alpha);
                else
                    v->fontFamily = OGUI::Lerp(sheet.Get<std::string>(prop.from), sheet.Get<std::string>(prop.to), prop.alpha);
                
                if(prevValue != v->fontFamily)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontStyle:{
                auto v = fget();
                auto prevValue = v->fontStyle;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->fontStyle = sheet.Get<TextStyle>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontStyle = sheet.Get<TextStyle>(prop.to);
                else if(prop.from == prop.to)
                    v->fontStyle = OGUI::Lerp(v->fontStyle, sheet.Get<TextStyle>(prop.to), prop.alpha);
                else
                    v->fontStyle = OGUI::Lerp(sheet.Get<TextStyle>(prop.from), sheet.Get<TextStyle>(prop.to), prop.alpha);
                
                if(prevValue != v->fontStyle)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontWeight:{
                auto v = fget();
                auto prevValue = v->fontWeight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->fontWeight = sheet.Get<int>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontWeight = sheet.Get<int>(prop.to);
                else if(prop.from == prop.to)
                    v->fontWeight = OGUI::Lerp(v->fontWeight, sheet.Get<int>(prop.to), prop.alpha);
                else
                    v->fontWeight = OGUI::Lerp(sheet.Get<int>(prop.from), sheet.Get<int>(prop.to), prop.alpha);
                
                if(prevValue != v->fontWeight)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::lineHeight:{
                auto v = fget();
                auto prevValue = v->lineHeight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->lineHeight = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->lineHeight = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->lineHeight = OGUI::Lerp(v->lineHeight, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->lineHeight = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->lineHeight)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textAlign:{
                auto v = fget();
                auto prevValue = v->textAlign;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->textAlign = sheet.Get<TextAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->textAlign = sheet.Get<TextAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->textAlign = OGUI::Lerp(v->textAlign, sheet.Get<TextAlign>(prop.to), prop.alpha);
                else
                    v->textAlign = OGUI::Lerp(sheet.Get<TextAlign>(prop.from), sheet.Get<TextAlign>(prop.to), prop.alpha);
                
                if(prevValue != v->textAlign)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            default: break;
        }
    }
    return damage;
}

bool OGUI::StyleText::ParseProperties(StyleSheetStorage& sheet, std::string_view prop, std::string_view value, StyleRule& rule, std::string& errorMsg)
{
    size_t phash = OGUI::hash(prop);

    StyleKeyword keyword = StyleKeyword::None;
    ParseValue(value, keyword);
    if(keyword != StyleKeyword::None)
    {
        switch(phash)
        {
            case Ids::fontSize:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::color:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::fontFamily:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::fontStyle:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::fontWeight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::lineHeight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::textAlign:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            default: break;
        }
        return false;
    }
    //longhands
    switch(phash)
    {
        case Ids::fontSize:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse font-size value!";
                return false;
            }
            return true;
        }
        case Ids::color:{
            Color4f v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<Color4f>(v)});
            else
            {
                errorMsg = "failed to parse color value!";
                return false;
            }
            return true;
        }
        case Ids::fontFamily:{
            std::string v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<std::string>(v)});
            else
            {
                errorMsg = "failed to parse font-family value!";
                return false;
            }
            return true;
        }
        case Ids::fontStyle:{
            TextStyle v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<TextStyle>(v)});
            else
            {
                errorMsg = "failed to parse font-style value!";
                return false;
            }
            return true;
        }
        case Ids::fontWeight:{
            int v;
            if(ParseTextWeight(value, v))
                rule.properties.push_back({phash, sheet.Push<int>(v)});
            else
            {
                errorMsg = "failed to parse font-weight value!";
                return false;
            }
            return true;
        }
        case Ids::lineHeight:{
            float v;
            if(ParseLineHeight(value, v))
                rule.properties.push_back({phash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse line-height value!";
                return false;
            }
            return true;
        }
        case Ids::textAlign:{
            TextAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<TextAlign>(v)});
            else
            {
                errorMsg = "failed to parse text-align value!";
                return false;
            }
            return true;
        }
        default: break;
    }
    return false;
}