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
    fontStyle = ETextStyle::Normal;
    fontWeight = 400;
    lineHeight = 1.f;
    textAlign = ETextAlign::Start;
    textShadow = {};
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
                    v->fontStyle = ETextStyle::Normal;
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
                    v->textAlign = ETextAlign::Start;
                    break;
                    }
                case Ids::textShadow:{
                    auto v = fget();
                    v->textShadow = {};
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
                case Ids::textShadow:{
                    auto v = fget();
                    v->textShadow = pst->textShadow;
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
                    v->fontStyle = sheet.Get<ETextStyle>(prop.value);
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
                    v->textAlign = sheet.Get<ETextAlign>(prop.value);
                    break;
                    }
                case Ids::textShadow:{
                    auto v = fget();
                    v->textShadow = ToOwned(sheet.Get<gsl::span<TextShadow>>(prop.value));
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
                    v->fontStyle = sheet.Get<ETextStyle>(prop.from);
                else if(prop.alpha == 1.f)
                    v->fontStyle = sheet.Get<ETextStyle>(prop.to);
                else if(prop.from == prop.to)
                    v->fontStyle = OGUI::Lerp(v->fontStyle, sheet.Get<ETextStyle>(prop.to), prop.alpha);
                else
                    v->fontStyle = OGUI::Lerp(sheet.Get<ETextStyle>(prop.from), sheet.Get<ETextStyle>(prop.to), prop.alpha);
                
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
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->textAlign = sheet.Get<ETextAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->textAlign = sheet.Get<ETextAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->textAlign = OGUI::Lerp(v->textAlign, sheet.Get<ETextAlign>(prop.to), prop.alpha);
                else
                    v->textAlign = OGUI::Lerp(sheet.Get<ETextAlign>(prop.from), sheet.Get<ETextAlign>(prop.to), prop.alpha);
                
                break;
                }
            case Ids::textShadow:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->textShadow = ToOwned(sheet.Get<gsl::span<TextShadow>>(prop.from));
                else if(prop.alpha == 1.f)
                    v->textShadow = ToOwned(sheet.Get<gsl::span<TextShadow>>(prop.to));
                else if(prop.from == prop.to)
                    v->textShadow = OGUI::Lerp(v->textShadow, sheet.Get<gsl::span<TextShadow>>(prop.to), prop.alpha);
                else
                    v->textShadow = OGUI::Lerp(sheet.Get<gsl::span<TextShadow>>(prop.from), sheet.Get<gsl::span<TextShadow>>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}

void OGUI::StyleText::SetupParser()
{
	{
        using namespace CSSParser;
        std::string grammar = "font-size <- 'font-size' _ ':' _ (GlobalValue / Number)";
        RegisterProperty("font-size");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontSize;
            parser["font-size"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "color <- 'color' _ ':' _ (GlobalValue / Color)";
        RegisterProperty("color");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::color;
            parser["color"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "font-family <- 'font-family' _ ':' _ (GlobalValue / String)";
        RegisterProperty("font-family");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontFamily;
            parser["font-family"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<std::string>(std::any_cast<std::string&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "font-style <- 'font-style' _ ':' _ (GlobalValue / ETextStyle)";
        RegisterProperty("font-style");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontStyle;
            parser["font-style"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "font-weight <- 'font-weight' _ ':' _ (GlobalValue / TextWeight)";
        RegisterProperty("font-weight");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::fontWeight;
            parser["font-weight"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "line-height <- 'line-height' _ ':' _ (GlobalValue / TextLineHeight)";
        RegisterProperty("line-height");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::lineHeight;
            parser["line-height"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "text-align <- 'text-align' _ ':' _ (GlobalValue / ETextAlign)";
        RegisterProperty("text-align");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textAlign;
            parser["text-align"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "text-shadow <- 'text-shadow' _ ':' _ (GlobalValue / TextShadow)";
        RegisterProperty("text-shadow");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::textShadow;
            parser["text-shadow"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<gsl::span<TextShadow>>(std::any_cast<std::vector<TextShadow>&>(vs[0]))});
            };
        });
    }
}