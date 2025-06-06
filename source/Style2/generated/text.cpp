//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"

size_t StyleTextEntry = 0;

size_t OGUI::StyleText::GetEntry()
{
    return StyleTextEntry;
}

void OGUI::StyleText::SetEntry(size_t e)
{
    StyleTextEntry = e;
}

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
    auto& s = style.structs[StyleTextEntry];
    return (StyleText*)s.ptr.get();
}

OGUI::StyleText& OGUI::StyleText::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StyleTextEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StyleText>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StyleText>(*(OGUI::StyleText*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(StyleText*)s.ptr.get();
}

void OGUI::StyleText::Dispose(ComputedStyle& style)
{
    style.structs[StyleTextEntry].ptr.reset();
}

void OGUI::StyleText::Initialize()
{
    fontSize = 20.f;
    color = Color4f(0,0,0,1);
    fontFamily = {};
    fontStyle = ETextStyle::Normal;
    fontWeight = 400;
    lineHeight = YGValueAuto;
    textAlign = ETextAlign::Start;
    letterSpacing = 0.f;
    wordSpacing = 0.f;
    textShadow = {};
    textDecorationColor = Color4f(0,0,0,1);
    textDecorationLine = ETextDecorationLine::None;
    textDecorationThickness = 1;
    textJustify = ETextJustify::InterWord;
}

void OGUI::StyleText::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    auto mask = override[StyleTextEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::fontSize: if(mask & (1ull<<0)) continue; break;
            case Ids::color: if(mask & (1ull<<1)) continue; break;
            case Ids::fontFamily: if(mask & (1ull<<2)) continue; break;
            case Ids::fontStyle: if(mask & (1ull<<3)) continue; break;
            case Ids::fontWeight: if(mask & (1ull<<4)) continue; break;
            case Ids::lineHeight: if(mask & (1ull<<5)) continue; break;
            case Ids::textAlign: if(mask & (1ull<<6)) continue; break;
            case Ids::letterSpacing: if(mask & (1ull<<7)) continue; break;
            case Ids::wordSpacing: if(mask & (1ull<<8)) continue; break;
            case Ids::textShadow: if(mask & (1ull<<9)) continue; break;
            case Ids::textDecorationColor: if(mask & (1ull<<10)) continue; break;
            case Ids::textDecorationLine: if(mask & (1ull<<11)) continue; break;
            case Ids::textDecorationThickness: if(mask & (1ull<<12)) continue; break;
            case Ids::textJustify: if(mask & (1ull<<13)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
             )
            {
                switch(prop.id)
                {
                case Ids::fontSize: {
                    auto& v = GetOrAdd(style);
                    v.fontSize = 20.f;
                    break;
                    }
                case Ids::color: {
                    auto& v = GetOrAdd(style);
                    v.color = Color4f(0,0,0,1);
                    break;
                    }
                case Ids::fontFamily: {
                    auto& v = GetOrAdd(style);
                    v.fontFamily = {};
                    break;
                    }
                case Ids::fontStyle: {
                    auto& v = GetOrAdd(style);
                    v.fontStyle = ETextStyle::Normal;
                    break;
                    }
                case Ids::fontWeight: {
                    auto& v = GetOrAdd(style);
                    v.fontWeight = 400;
                    break;
                    }
                case Ids::lineHeight: {
                    auto& v = GetOrAdd(style);
                    v.lineHeight = YGValueAuto;
                    break;
                    }
                case Ids::textAlign: {
                    auto& v = GetOrAdd(style);
                    v.textAlign = ETextAlign::Start;
                    break;
                    }
                case Ids::letterSpacing: {
                    auto& v = GetOrAdd(style);
                    v.letterSpacing = 0.f;
                    break;
                    }
                case Ids::wordSpacing: {
                    auto& v = GetOrAdd(style);
                    v.wordSpacing = 0.f;
                    break;
                    }
                case Ids::textShadow: {
                    auto& v = GetOrAdd(style);
                    v.textShadow = {};
                    break;
                    }
                case Ids::textDecorationColor: {
                    auto& v = GetOrAdd(style);
                    v.textDecorationColor = Color4f(0,0,0,1);
                    break;
                    }
                case Ids::textDecorationLine: {
                    auto& v = GetOrAdd(style);
                    v.textDecorationLine = ETextDecorationLine::None;
                    break;
                    }
                case Ids::textDecorationThickness: {
                    auto& v = GetOrAdd(style);
                    v.textDecorationThickness = 1;
                    break;
                    }
                case Ids::textJustify: {
                    auto& v = GetOrAdd(style);
                    v.textJustify = ETextJustify::InterWord;
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
                    auto& v = GetOrAdd(style);
                    v.fontSize = pst->fontSize;
                    break;
                    }
                case Ids::color:{
                    auto& v = GetOrAdd(style);
                    v.color = pst->color;
                    break;
                    }
                case Ids::fontFamily:{
                    auto& v = GetOrAdd(style);
                    v.fontFamily = pst->fontFamily;
                    break;
                    }
                case Ids::fontStyle:{
                    auto& v = GetOrAdd(style);
                    v.fontStyle = pst->fontStyle;
                    break;
                    }
                case Ids::fontWeight:{
                    auto& v = GetOrAdd(style);
                    v.fontWeight = pst->fontWeight;
                    break;
                    }
                case Ids::lineHeight:{
                    auto& v = GetOrAdd(style);
                    v.lineHeight = pst->lineHeight;
                    break;
                    }
                case Ids::textAlign:{
                    auto& v = GetOrAdd(style);
                    v.textAlign = pst->textAlign;
                    break;
                    }
                case Ids::letterSpacing:{
                    auto& v = GetOrAdd(style);
                    v.letterSpacing = pst->letterSpacing;
                    break;
                    }
                case Ids::wordSpacing:{
                    auto& v = GetOrAdd(style);
                    v.wordSpacing = pst->wordSpacing;
                    break;
                    }
                case Ids::textShadow:{
                    auto& v = GetOrAdd(style);
                    v.textShadow = pst->textShadow;
                    break;
                    }
                case Ids::textDecorationColor:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationColor = pst->textDecorationColor;
                    break;
                    }
                case Ids::textDecorationLine:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationLine = pst->textDecorationLine;
                    break;
                    }
                case Ids::textDecorationThickness:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationThickness = pst->textDecorationThickness;
                    break;
                    }
                case Ids::textJustify:{
                    auto& v = GetOrAdd(style);
                    v.textJustify = pst->textJustify;
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
                    auto& v = GetOrAdd(style);
                    v.fontSize = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::color:{
                    auto& v = GetOrAdd(style);
                    v.color = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Ids::fontFamily:{
                    auto& v = GetOrAdd(style);
                    v.fontFamily = ToOwned(sheet.Get<const gsl::span<const ostr::string>>(prop.value));
                    break;
                    }
                case Ids::fontStyle:{
                    auto& v = GetOrAdd(style);
                    v.fontStyle = sheet.Get<ETextStyle>(prop.value);
                    break;
                    }
                case Ids::fontWeight:{
                    auto& v = GetOrAdd(style);
                    v.fontWeight = sheet.Get<int>(prop.value);
                    break;
                    }
                case Ids::lineHeight:{
                    auto& v = GetOrAdd(style);
                    v.lineHeight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::textAlign:{
                    auto& v = GetOrAdd(style);
                    v.textAlign = sheet.Get<ETextAlign>(prop.value);
                    break;
                    }
                case Ids::letterSpacing:{
                    auto& v = GetOrAdd(style);
                    v.letterSpacing = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::wordSpacing:{
                    auto& v = GetOrAdd(style);
                    v.wordSpacing = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::textShadow:{
                    auto& v = GetOrAdd(style);
                    v.textShadow = ToOwned(sheet.Get<const gsl::span<const TextShadow>>(prop.value));
                    break;
                    }
                case Ids::textDecorationColor:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationColor = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Ids::textDecorationLine:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationLine = sheet.Get<ETextDecorationLine>(prop.value);
                    break;
                    }
                case Ids::textDecorationThickness:{
                    auto& v = GetOrAdd(style);
                    v.textDecorationThickness = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::textJustify:{
                    auto& v = GetOrAdd(style);
                    v.textJustify = sheet.Get<ETextJustify>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleText::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleTextEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::fontSize: if(mask & (1ull<<0)) continue; break;
            case Ids::color: if(mask & (1ull<<1)) continue; break;
            case Ids::fontFamily: if(mask & (1ull<<2)) continue; break;
            case Ids::fontStyle: if(mask & (1ull<<3)) continue; break;
            case Ids::fontWeight: if(mask & (1ull<<4)) continue; break;
            case Ids::lineHeight: if(mask & (1ull<<5)) continue; break;
            case Ids::textAlign: if(mask & (1ull<<6)) continue; break;
            case Ids::letterSpacing: if(mask & (1ull<<7)) continue; break;
            case Ids::wordSpacing: if(mask & (1ull<<8)) continue; break;
            case Ids::textShadow: if(mask & (1ull<<9)) continue; break;
            case Ids::textDecorationColor: if(mask & (1ull<<10)) continue; break;
            case Ids::textDecorationLine: if(mask & (1ull<<11)) continue; break;
            case Ids::textDecorationThickness: if(mask & (1ull<<12)) continue; break;
            case Ids::textJustify: if(mask & (1ull<<13)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::fontSize:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontSize;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> float
                {
                    if(!keyword)
                        return sheet.Get<float>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().fontSize;
                    }
                    else
                    { 
                        return GetDefault().fontSize;
                    }
                };
                if(prop.alpha == 0.f)
                    v.fontSize = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.fontSize = getValue(prop.to, prop.toKeyword);
                else
                    v.fontSize = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.fontSize)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::color:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> Color4f
                {
                    if(!keyword)
                        return sheet.Get<Color4f>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().color;
                    }
                    else
                    { 
                        return GetDefault().color;
                    }
                };
                if(prop.alpha == 0.f)
                    v.color = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.color = getValue(prop.to, prop.toKeyword);
                else
                    v.color = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::fontFamily:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> const gsl::span<const ostr::string>
                {
                    if(!keyword)
                        return sheet.Get<const gsl::span<const ostr::string>>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().fontFamily;
                    }
                    else
                    { 
                        return GetDefault().fontFamily;
                    }
                };
                if(prop.alpha == 0.f)
                    v.fontFamily = ToOwned(getValue(prop.from, prop.fromKeyword));
                else if(prop.alpha == 1.f)
                    v.fontFamily = ToOwned(getValue(prop.to, prop.toKeyword));
                else
                    v.fontFamily = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontStyle:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontStyle;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> ETextStyle
                {
                    if(!keyword)
                        return sheet.Get<ETextStyle>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().fontStyle;
                    }
                    else
                    { 
                        return GetDefault().fontStyle;
                    }
                };
                if(prop.alpha == 0.f)
                    v.fontStyle = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.fontStyle = getValue(prop.to, prop.toKeyword);
                else
                    v.fontStyle = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.fontStyle)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontWeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontWeight;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> int
                {
                    if(!keyword)
                        return sheet.Get<int>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().fontWeight;
                    }
                    else
                    { 
                        return GetDefault().fontWeight;
                    }
                };
                if(prop.alpha == 0.f)
                    v.fontWeight = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.fontWeight = getValue(prop.to, prop.toKeyword);
                else
                    v.fontWeight = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.fontWeight)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::lineHeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.lineHeight;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> YGValue
                {
                    if(!keyword)
                        return sheet.Get<YGValue>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().lineHeight;
                    }
                    else
                    { 
                        return GetDefault().lineHeight;
                    }
                };
                if(prop.alpha == 0.f)
                    v.lineHeight = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.lineHeight = getValue(prop.to, prop.toKeyword);
                else
                    v.lineHeight = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.lineHeight)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textAlign:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> ETextAlign
                {
                    if(!keyword)
                        return sheet.Get<ETextAlign>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textAlign;
                    }
                    else
                    { 
                        return GetDefault().textAlign;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textAlign = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.textAlign = getValue(prop.to, prop.toKeyword);
                else
                    v.textAlign = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::letterSpacing:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.letterSpacing;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> float
                {
                    if(!keyword)
                        return sheet.Get<float>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().letterSpacing;
                    }
                    else
                    { 
                        return GetDefault().letterSpacing;
                    }
                };
                if(prop.alpha == 0.f)
                    v.letterSpacing = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.letterSpacing = getValue(prop.to, prop.toKeyword);
                else
                    v.letterSpacing = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.letterSpacing)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::wordSpacing:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.wordSpacing;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> float
                {
                    if(!keyword)
                        return sheet.Get<float>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().wordSpacing;
                    }
                    else
                    { 
                        return GetDefault().wordSpacing;
                    }
                };
                if(prop.alpha == 0.f)
                    v.wordSpacing = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.wordSpacing = getValue(prop.to, prop.toKeyword);
                else
                    v.wordSpacing = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.wordSpacing)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textShadow:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> const gsl::span<const TextShadow>
                {
                    if(!keyword)
                        return sheet.Get<const gsl::span<const TextShadow>>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textShadow;
                    }
                    else
                    { 
                        return GetDefault().textShadow;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textShadow = ToOwned(getValue(prop.from, prop.fromKeyword));
                else if(prop.alpha == 1.f)
                    v.textShadow = ToOwned(getValue(prop.to, prop.toKeyword));
                else
                    v.textShadow = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::textDecorationColor:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationColor;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> Color4f
                {
                    if(!keyword)
                        return sheet.Get<Color4f>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textDecorationColor;
                    }
                    else
                    { 
                        return GetDefault().textDecorationColor;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textDecorationColor = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.textDecorationColor = getValue(prop.to, prop.toKeyword);
                else
                    v.textDecorationColor = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.textDecorationColor)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textDecorationLine:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationLine;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> ETextDecorationLine
                {
                    if(!keyword)
                        return sheet.Get<ETextDecorationLine>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textDecorationLine;
                    }
                    else
                    { 
                        return GetDefault().textDecorationLine;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textDecorationLine = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.textDecorationLine = getValue(prop.to, prop.toKeyword);
                else
                    v.textDecorationLine = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.textDecorationLine)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textDecorationThickness:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationThickness;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> float
                {
                    if(!keyword)
                        return sheet.Get<float>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textDecorationThickness;
                    }
                    else
                    { 
                        return GetDefault().textDecorationThickness;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textDecorationThickness = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.textDecorationThickness = getValue(prop.to, prop.toKeyword);
                else
                    v.textDecorationThickness = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.textDecorationThickness)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textJustify:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textJustify;
                auto getValue = [&](VariantHandle& handle, bool keyword) -> ETextJustify
                {
                    if(!keyword)
                        return sheet.Get<ETextJustify>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    )
                    {
                        return GetDefault().textJustify;
                    }
                    else
                    { 
                        return GetDefault().textJustify;
                    }
                };
                if(prop.alpha == 0.f)
                    v.textJustify = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.textJustify = getValue(prop.to, prop.toKeyword);
                else
                    v.textJustify = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                if(prevValue != v.textJustify)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            default: break;
        }
    }
    return damage;
}


OGUI::RestyleDamage OGUI::StyleText::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::fontSize:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontSize;
                if(prop.alpha == 1.f)
                    v.fontSize = dst.fontSize;
                else
                    v.fontSize = OGUI::Lerp(src.fontSize, dst.fontSize, prop.alpha);
                
                if(prevValue != v.fontSize)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::color:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.color = dst.color;
                else
                    v.color = OGUI::Lerp(src.color, dst.color, prop.alpha);
                
                break;
                }
            case Ids::fontFamily:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.fontFamily = dst.fontFamily;
                else
                    v.fontFamily = OGUI::Lerp(src.fontFamily, dst.fontFamily, prop.alpha);
                
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontStyle:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontStyle;
                if(prop.alpha == 1.f)
                    v.fontStyle = dst.fontStyle;
                else
                    v.fontStyle = OGUI::Lerp(src.fontStyle, dst.fontStyle, prop.alpha);
                
                if(prevValue != v.fontStyle)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontWeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontWeight;
                if(prop.alpha == 1.f)
                    v.fontWeight = dst.fontWeight;
                else
                    v.fontWeight = OGUI::Lerp(src.fontWeight, dst.fontWeight, prop.alpha);
                
                if(prevValue != v.fontWeight)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::lineHeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.lineHeight;
                if(prop.alpha == 1.f)
                    v.lineHeight = dst.lineHeight;
                else
                    v.lineHeight = OGUI::Lerp(src.lineHeight, dst.lineHeight, prop.alpha);
                
                if(prevValue != v.lineHeight)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textAlign:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.textAlign = dst.textAlign;
                else
                    v.textAlign = OGUI::Lerp(src.textAlign, dst.textAlign, prop.alpha);
                
                break;
                }
            case Ids::letterSpacing:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.letterSpacing;
                if(prop.alpha == 1.f)
                    v.letterSpacing = dst.letterSpacing;
                else
                    v.letterSpacing = OGUI::Lerp(src.letterSpacing, dst.letterSpacing, prop.alpha);
                
                if(prevValue != v.letterSpacing)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::wordSpacing:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.wordSpacing;
                if(prop.alpha == 1.f)
                    v.wordSpacing = dst.wordSpacing;
                else
                    v.wordSpacing = OGUI::Lerp(src.wordSpacing, dst.wordSpacing, prop.alpha);
                
                if(prevValue != v.wordSpacing)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textShadow:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.textShadow = dst.textShadow;
                else
                    v.textShadow = OGUI::Lerp(src.textShadow, dst.textShadow, prop.alpha);
                
                break;
                }
            case Ids::textDecorationColor:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationColor;
                if(prop.alpha == 1.f)
                    v.textDecorationColor = dst.textDecorationColor;
                else
                    v.textDecorationColor = OGUI::Lerp(src.textDecorationColor, dst.textDecorationColor, prop.alpha);
                
                if(prevValue != v.textDecorationColor)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textDecorationLine:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationLine;
                if(prop.alpha == 1.f)
                    v.textDecorationLine = dst.textDecorationLine;
                else
                    v.textDecorationLine = OGUI::Lerp(src.textDecorationLine, dst.textDecorationLine, prop.alpha);
                
                if(prevValue != v.textDecorationLine)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textDecorationThickness:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textDecorationThickness;
                if(prop.alpha == 1.f)
                    v.textDecorationThickness = dst.textDecorationThickness;
                else
                    v.textDecorationThickness = OGUI::Lerp(src.textDecorationThickness, dst.textDecorationThickness, prop.alpha);
                
                if(prevValue != v.textDecorationThickness)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textJustify:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.textJustify;
                if(prop.alpha == 1.f)
                    v.textJustify = dst.textJustify;
                else
                    v.textJustify = OGUI::Lerp(src.textJustify, dst.textJustify, prop.alpha);
                
                if(prevValue != v.textJustify)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            default: break;
        }
    }
    return damage; 
}

void OGUI::StyleText::Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StyleTextEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.fontSize = po->fontSize;
    if(mask & (1ull << 1))
        s.color = po->color;
    if(mask & (1ull << 2))
        s.fontFamily = po->fontFamily;
    if(mask & (1ull << 3))
        s.fontStyle = po->fontStyle;
    if(mask & (1ull << 4))
        s.fontWeight = po->fontWeight;
    if(mask & (1ull << 5))
        s.lineHeight = po->lineHeight;
    if(mask & (1ull << 6))
        s.textAlign = po->textAlign;
    if(mask & (1ull << 7))
        s.letterSpacing = po->letterSpacing;
    if(mask & (1ull << 8))
        s.wordSpacing = po->wordSpacing;
    if(mask & (1ull << 9))
        s.textShadow = po->textShadow;
    if(mask & (1ull << 10))
        s.textDecorationColor = po->textDecorationColor;
    if(mask & (1ull << 11))
        s.textDecorationLine = po->textDecorationLine;
    if(mask & (1ull << 12))
        s.textDecorationThickness = po->textDecorationThickness;
    if(mask & (1ull << 13))
        s.textJustify = po->textJustify;
}

void OGUI::StyleText::MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    for(auto prop : override)
    {
        switch(prop)
        {
            case Ids::fontSize: {
                 auto& v = GetOrAdd(style);
                 v.fontSize = po->fontSize;
                 break;
            }
            case Ids::color: {
                 auto& v = GetOrAdd(style);
                 v.color = po->color;
                 break;
            }
            case Ids::fontFamily: {
                 auto& v = GetOrAdd(style);
                 v.fontFamily = po->fontFamily;
                 break;
            }
            case Ids::fontStyle: {
                 auto& v = GetOrAdd(style);
                 v.fontStyle = po->fontStyle;
                 break;
            }
            case Ids::fontWeight: {
                 auto& v = GetOrAdd(style);
                 v.fontWeight = po->fontWeight;
                 break;
            }
            case Ids::lineHeight: {
                 auto& v = GetOrAdd(style);
                 v.lineHeight = po->lineHeight;
                 break;
            }
            case Ids::textAlign: {
                 auto& v = GetOrAdd(style);
                 v.textAlign = po->textAlign;
                 break;
            }
            case Ids::letterSpacing: {
                 auto& v = GetOrAdd(style);
                 v.letterSpacing = po->letterSpacing;
                 break;
            }
            case Ids::wordSpacing: {
                 auto& v = GetOrAdd(style);
                 v.wordSpacing = po->wordSpacing;
                 break;
            }
            case Ids::textShadow: {
                 auto& v = GetOrAdd(style);
                 v.textShadow = po->textShadow;
                 break;
            }
            case Ids::textDecorationColor: {
                 auto& v = GetOrAdd(style);
                 v.textDecorationColor = po->textDecorationColor;
                 break;
            }
            case Ids::textDecorationLine: {
                 auto& v = GetOrAdd(style);
                 v.textDecorationLine = po->textDecorationLine;
                 break;
            }
            case Ids::textDecorationThickness: {
                 auto& v = GetOrAdd(style);
                 v.textDecorationThickness = po->textDecorationThickness;
                 break;
            }
            case Ids::textJustify: {
                 auto& v = GetOrAdd(style);
                 v.textJustify = po->textJustify;
                 break;
            }
        }
    }
}

size_t OGUI::StyleText::GetProperty(ostr::string_view pname)
{
    switchstr(pname)
    {
        casestr("font-size") return Ids::fontSize;
        casestr("color") return Ids::color;
        casestr("font-family") return Ids::fontFamily;
        casestr("font-style") return Ids::fontStyle;
        casestr("font-weight") return Ids::fontWeight;
        casestr("line-height") return Ids::lineHeight;
        casestr("text-align") return Ids::textAlign;
        casestr("letter-spacing") return Ids::letterSpacing;
        casestr("word-spacing") return Ids::wordSpacing;
        casestr("text-shadow") return Ids::textShadow;
        casestr("text-decoration-color") return Ids::textDecorationColor;
        casestr("text-decoration-line") return Ids::textDecorationLine;
        casestr("text-decoration-thickness") return Ids::textDecorationThickness;
        casestr("text-justify") return Ids::textJustify;
        default: return -1;
    }
    return -1;
}

void OGUI::StyleText::SetupParser()
{
	{
        using namespace CSSParser;
        static const auto grammar = "font-sizeValue <- GlobalValue / Number \nfont-size <- 'font-size' _ ':' _ font-sizeValue";
        RegisterProperty("font-size");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontSize;
            parser["font-sizeValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "colorValue <- GlobalValue / Color \ncolor <- 'color' _ ':' _ colorValue";
        RegisterProperty("color");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::color;
            parser["colorValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<Color4f>(std::any_cast<Color4f&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "font-familyValue <- GlobalValue / (FontFamily (_ ',' _ FontFamily)*) \nfont-family <- 'font-family' _ ':' _ font-familyValue";
        RegisterProperty("font-family");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontFamily;
            parser["font-familyValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                {
                    std::vector<ostr::string> value;
                    for(auto& e : vs)
                        value.emplace_back(ostr::string::decode_from_utf8(any_move<const std::string_view>(e)));
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const gsl::span<const ostr::string>>(value)});
                }
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "font-styleValue <- GlobalValue / TextStyle \nfont-style <- 'font-style' _ ':' _ font-styleValue";
        RegisterProperty("font-style");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontStyle;
            parser["font-styleValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<ETextStyle>(std::any_cast<ETextStyle&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "font-weightValue <- GlobalValue / TextWeight \nfont-weight <- 'font-weight' _ ':' _ font-weightValue";
        RegisterProperty("font-weight");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontWeight;
            parser["font-weightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<int>(std::any_cast<int&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "line-heightValue <- GlobalValue / Width \nline-height <- 'line-height' _ ':' _ line-heightValue";
        RegisterProperty("line-height");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::lineHeight;
            parser["line-heightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-alignValue <- GlobalValue / TextAlign \ntext-align <- 'text-align' _ ':' _ text-alignValue";
        RegisterProperty("text-align");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textAlign;
            parser["text-alignValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<ETextAlign>(std::any_cast<ETextAlign&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "letter-spacingValue <- GlobalValue / Length \nletter-spacing <- 'letter-spacing' _ ':' _ letter-spacingValue";
        RegisterProperty("letter-spacing");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::letterSpacing;
            parser["letter-spacingValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "word-spacingValue <- GlobalValue / Length \nword-spacing <- 'word-spacing' _ ':' _ word-spacingValue";
        RegisterProperty("word-spacing");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::wordSpacing;
            parser["word-spacingValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-shadowValue <- GlobalValue / (TextShadow (_ ',' _ TextShadow)*) \ntext-shadow <- 'text-shadow' _ ':' _ text-shadowValue";
        RegisterProperty("text-shadow");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textShadow;
            parser["text-shadowValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                {
                    std::vector<TextShadow> value;
                    for(auto& e : vs)
                        value.emplace_back(any_move<TextShadow>(e));
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const gsl::span<const TextShadow>>(value)});
                }
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-decoration-colorValue <- GlobalValue / Color \ntext-decoration-color <- 'text-decoration-color' _ ':' _ text-decoration-colorValue";
        RegisterProperty("text-decoration-color");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textDecorationColor;
            parser["text-decoration-colorValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<Color4f>(std::any_cast<Color4f&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-decoration-lineValue <- GlobalValue / TextDecorationLine \ntext-decoration-line <- 'text-decoration-line' _ ':' _ text-decoration-lineValue";
        RegisterProperty("text-decoration-line");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textDecorationLine;
            parser["text-decoration-lineValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<ETextDecorationLine>(std::any_cast<ETextDecorationLine&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-decoration-thicknessValue <- GlobalValue / Number \ntext-decoration-thickness <- 'text-decoration-thickness' _ ':' _ text-decoration-thicknessValue";
        RegisterProperty("text-decoration-thickness");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textDecorationThickness;
            parser["text-decoration-thicknessValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "text-justifyValue <- GlobalValue / TextJustify \ntext-justify <- 'text-justify' _ ':' _ text-justifyValue";
        RegisterProperty("text-justify");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textJustify;
            parser["text-justifyValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<ETextJustify>(std::any_cast<ETextJustify&>(vs[0]))});
            };
        });
    }
}


void OGUI::SetStyleFontSize(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<0;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::fontSize)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.fontSize = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).fontSize = override.fontSize;
        StyleText::GetOrAdd(element->_transitionSrcStyle).fontSize = StyleText::Get(element->_style).fontSize;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).fontSize = override.fontSize;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleFontSize(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<0);
}
void OGUI::SetStyleColor(VisualElement* element, const Color4f& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<1;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::color)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.color = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).color = override.color;
        StyleText::GetOrAdd(element->_transitionSrcStyle).color = StyleText::Get(element->_style).color;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).color = override.color;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleColor(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<1);
}
void OGUI::SetStyleFontFamily(VisualElement* element, const gsl::span<const ostr::string>& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<2;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::fontFamily)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.fontFamily = ToOwned(value);
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).fontFamily = override.fontFamily;
        StyleText::GetOrAdd(element->_transitionSrcStyle).fontFamily = StyleText::Get(element->_style).fontFamily;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).fontFamily = override.fontFamily;
        RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleFontFamily(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<2);
}
void OGUI::SetStyleFontStyle(VisualElement* element, const ETextStyle& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<3;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::fontStyle)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.fontStyle = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).fontStyle = override.fontStyle;
        StyleText::GetOrAdd(element->_transitionSrcStyle).fontStyle = StyleText::Get(element->_style).fontStyle;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).fontStyle = override.fontStyle;
        RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleFontStyle(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<3);
}
void OGUI::SetStyleFontWeight(VisualElement* element, const int& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<4;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::fontWeight)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.fontWeight = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).fontWeight = override.fontWeight;
        StyleText::GetOrAdd(element->_transitionSrcStyle).fontWeight = StyleText::Get(element->_style).fontWeight;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).fontWeight = override.fontWeight;
        RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleFontWeight(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<4);
}
void OGUI::SetStyleLineHeight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<5;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::lineHeight)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.lineHeight = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).lineHeight = override.lineHeight;
        StyleText::GetOrAdd(element->_transitionSrcStyle).lineHeight = StyleText::Get(element->_style).lineHeight;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).lineHeight = override.lineHeight;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::SetStyleLineHeightPixel(VisualElement* element, float value)
{
    SetStyleLineHeight(element, YGValue{value, YGUnitPoint});
}
void OGUI::SetStyleLineHeightPercentage(VisualElement* element, float value)
{
    SetStyleLineHeight(element, YGValue{value, YGUnitPercent});
}
void OGUI::SetStyleLineHeightAuto(VisualElement* element)
{
    SetStyleLineHeight(element, YGValueAuto);
}
void OGUI::ResetStyleLineHeight(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<5);
}
void OGUI::SetStyleTextAlign(VisualElement* element, const ETextAlign& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<6;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textAlign)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textAlign = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textAlign = override.textAlign;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textAlign = StyleText::Get(element->_style).textAlign;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textAlign = override.textAlign;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextAlign(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<6);
}
void OGUI::SetStyleLetterSpacing(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<7;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::letterSpacing)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.letterSpacing = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).letterSpacing = override.letterSpacing;
        StyleText::GetOrAdd(element->_transitionSrcStyle).letterSpacing = StyleText::Get(element->_style).letterSpacing;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).letterSpacing = override.letterSpacing;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleLetterSpacing(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<7);
}
void OGUI::SetStyleWordSpacing(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<8;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::wordSpacing)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.wordSpacing = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).wordSpacing = override.wordSpacing;
        StyleText::GetOrAdd(element->_transitionSrcStyle).wordSpacing = StyleText::Get(element->_style).wordSpacing;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).wordSpacing = override.wordSpacing;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleWordSpacing(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<8);
}
void OGUI::SetStyleTextShadow(VisualElement* element, const gsl::span<const TextShadow>& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<9;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textShadow)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textShadow = ToOwned(value);
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textShadow = override.textShadow;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textShadow = StyleText::Get(element->_style).textShadow;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textShadow = override.textShadow;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextShadow(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<9);
}
void OGUI::SetStyleTextDecorationColor(VisualElement* element, const Color4f& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<10;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textDecorationColor)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textDecorationColor = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textDecorationColor = override.textDecorationColor;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textDecorationColor = StyleText::Get(element->_style).textDecorationColor;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textDecorationColor = override.textDecorationColor;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextDecorationColor(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<10);
}
void OGUI::SetStyleTextDecorationLine(VisualElement* element, const ETextDecorationLine& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<11;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textDecorationLine)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textDecorationLine = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textDecorationLine = override.textDecorationLine;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textDecorationLine = StyleText::Get(element->_style).textDecorationLine;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textDecorationLine = override.textDecorationLine;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextDecorationLine(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<11);
}
void OGUI::SetStyleTextDecorationThickness(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<12;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textDecorationThickness)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textDecorationThickness = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textDecorationThickness = override.textDecorationThickness;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textDecorationThickness = StyleText::Get(element->_style).textDecorationThickness;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textDecorationThickness = override.textDecorationThickness;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextDecorationThickness(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<12);
}
void OGUI::SetStyleTextJustify(VisualElement* element, const ETextJustify& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<13;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleText::Ids::textJustify)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleText::GetOrAdd(element->_overrideStyle);
    override.textJustify = value;
    if(transition)
    {
        StyleText::GetOrAdd(element->_transitionDstStyle).textJustify = override.textJustify;
        StyleText::GetOrAdd(element->_transitionSrcStyle).textJustify = StyleText::Get(element->_style).textJustify;
        transition->time = 0.f;
    }
    else
    {
        StyleText::GetOrAdd(element->_style).textJustify = override.textJustify;
        RestyleDamage damage = RestyleDamage::TextLayout;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleTextJustify(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<13);
}
