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
    textShadow = {};
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
            case Ids::textShadow: if(mask & (1ull<<7)) continue; break;
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
                case Ids::textShadow: {
                    auto& v = GetOrAdd(style);
                    v.textShadow = {};
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
                case Ids::textShadow:{
                    auto& v = GetOrAdd(style);
                    v.textShadow = pst->textShadow;
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
                case Ids::textShadow:{
                    auto& v = GetOrAdd(style);
                    v.textShadow = ToOwned(sheet.Get<const gsl::span<const TextShadow>>(prop.value));
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
            case Ids::textShadow: if(mask & (1ull<<7)) continue; break;
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
            case Ids::textShadow:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.textShadow = dst.textShadow;
                else
                    v.textShadow = OGUI::Lerp(src.textShadow, dst.textShadow, prop.alpha);
                
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
        s.textShadow = po->textShadow;
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
            case Ids::textShadow: {
                 auto& v = GetOrAdd(style);
                 v.textShadow = po->textShadow;
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
        casestr("text-shadow") return Ids::textShadow;
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
void OGUI::SetStyleTextShadow(VisualElement* element, const gsl::span<const TextShadow>& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<7;
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
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<7);
}
