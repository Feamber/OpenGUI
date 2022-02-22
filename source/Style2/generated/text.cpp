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
                    v.fontFamily = ToOwned(sheet.Get<const gsl::span<ostr::string>>(prop.value));
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
                    v.textShadow = ToOwned(sheet.Get<const gsl::span<TextShadow>>(prop.value));
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
                if(prop.alpha == 0.f)
                    v.fontSize = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v.fontSize = sheet.Get<float>(prop.to);
                else
                    v.fontSize = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v.fontSize)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::color:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 0.f)
                    v.color = sheet.Get<Color4f>(prop.from);
                else if(prop.alpha == 1.f)
                    v.color = sheet.Get<Color4f>(prop.to);
                else
                    v.color = OGUI::Lerp(sheet.Get<Color4f>(prop.from), sheet.Get<Color4f>(prop.to), prop.alpha);
                
                break;
                }
            case Ids::fontFamily:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 0.f)
                    v.fontFamily = ToOwned(sheet.Get<const gsl::span<ostr::string>>(prop.from));
                else if(prop.alpha == 1.f)
                    v.fontFamily = ToOwned(sheet.Get<const gsl::span<ostr::string>>(prop.to));
                else
                    v.fontFamily = OGUI::Lerp(sheet.Get<const gsl::span<ostr::string>>(prop.from), sheet.Get<const gsl::span<ostr::string>>(prop.to), prop.alpha);
                
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontStyle:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontStyle;
                if(prop.alpha == 0.f)
                    v.fontStyle = sheet.Get<ETextStyle>(prop.from);
                else if(prop.alpha == 1.f)
                    v.fontStyle = sheet.Get<ETextStyle>(prop.to);
                else
                    v.fontStyle = OGUI::Lerp(sheet.Get<ETextStyle>(prop.from), sheet.Get<ETextStyle>(prop.to), prop.alpha);
                
                if(prevValue != v.fontStyle)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::fontWeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.fontWeight;
                if(prop.alpha == 0.f)
                    v.fontWeight = sheet.Get<int>(prop.from);
                else if(prop.alpha == 1.f)
                    v.fontWeight = sheet.Get<int>(prop.to);
                else
                    v.fontWeight = OGUI::Lerp(sheet.Get<int>(prop.from), sheet.Get<int>(prop.to), prop.alpha);
                
                if(prevValue != v.fontWeight)
                    damage |= RestyleDamage::TextLayout|RestyleDamage::Font;
                break;
                }
            case Ids::lineHeight:{
                auto& v = GetOrAdd(style);
                auto prevValue = v.lineHeight;
                if(prop.alpha == 0.f)
                    v.lineHeight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v.lineHeight = sheet.Get<YGValue>(prop.to);
                else
                    v.lineHeight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v.lineHeight)
                    damage |= RestyleDamage::TextLayout;
                break;
                }
            case Ids::textAlign:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 0.f)
                    v.textAlign = sheet.Get<ETextAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v.textAlign = sheet.Get<ETextAlign>(prop.to);
                else
                    v.textAlign = OGUI::Lerp(sheet.Get<ETextAlign>(prop.from), sheet.Get<ETextAlign>(prop.to), prop.alpha);
                
                break;
                }
            case Ids::textShadow:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 0.f)
                    v.textShadow = ToOwned(sheet.Get<const gsl::span<TextShadow>>(prop.from));
                else if(prop.alpha == 1.f)
                    v.textShadow = ToOwned(sheet.Get<const gsl::span<TextShadow>>(prop.to));
                else
                    v.textShadow = OGUI::Lerp(sheet.Get<const gsl::span<TextShadow>>(prop.from), sheet.Get<const gsl::span<TextShadow>>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}


OGUI::RestyleDamage OGUI::StyleText::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleTextEntry];
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

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
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const gsl::span<ostr::string>>(value)});
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
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const gsl::span<TextShadow>>(value)});
                }
            };
        });
    }
}


void OGUI::SetStyleFontSize(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<0;
    StyleText::GetOrAdd(element->_style).fontSize = value;
    RestyleDamage damage = RestyleDamage::TextLayout;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleFontSize(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<0);
}
void OGUI::SetStyleColor(VisualElement* element, const Color4f& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<1;
    StyleText::GetOrAdd(element->_style).color = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleColor(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<1);
}
void OGUI::SetStyleFontFamily(VisualElement* element, const gsl::span<ostr::string>& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<2;
    StyleText::GetOrAdd(element->_style).fontFamily = ToOwned(value);
    RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleFontFamily(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<2);
}
void OGUI::SetStyleFontStyle(VisualElement* element, const ETextStyle& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<3;
    StyleText::GetOrAdd(element->_style).fontStyle = value;
    RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleFontStyle(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<3);
}
void OGUI::SetStyleFontWeight(VisualElement* element, const int& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<4;
    StyleText::GetOrAdd(element->_style).fontWeight = value;
    RestyleDamage damage = RestyleDamage::TextLayout|RestyleDamage::Font;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleFontWeight(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<4);
}
void OGUI::SetStyleLineHeight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<5;
    StyleText::GetOrAdd(element->_style).lineHeight = value;
    RestyleDamage damage = RestyleDamage::TextLayout;
    element->UpdateStyle(damage);
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
    StyleText::GetOrAdd(element->_style).textAlign = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleTextAlign(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<6);
}
void OGUI::SetStyleTextShadow(VisualElement* element, const gsl::span<TextShadow>& value)
{
    element->_procedureOverrides[StyleTextEntry] |= 1ull<<7;
    StyleText::GetOrAdd(element->_style).textShadow = ToOwned(value);
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleTextShadow(VisualElement* element)
{
    element->_procedureOverrides[StyleTextEntry] &= ~(1ull<<7);
}
