//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


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
    borderTopWidth = 0.f;
    borderRightWidth = 0.f;
    borderBottomWidth = 0.f;
    borderLeftWidth = 0.f;
    borderTopLeftRadius = LengthPercentage;
    borderTopRightRadius = LengthPercentage;
    borderBottomRightRadius = LengthPercentage;
    borderBottomLeftRadius = LengthPercentage;
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
                case Ids::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = 0.f;
                    break;
                    }
                case Ids::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = 0.f;
                    break;
                    }
                case Ids::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = 0.f;
                    break;
                    }
                case Ids::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = 0.f;
                    break;
                    }
                case Ids::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = LengthPercentage;
                    break;
                    }
                case Ids::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = LengthPercentage;
                    break;
                    }
                case Ids::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = LengthPercentage;
                    break;
                    }
                case Ids::borderBottomLeftRadius:{
                    auto v = fget();
                    v->borderBottomLeftRadius = LengthPercentage;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = pst->borderTopWidth;
                    break;
                    }
                case Ids::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = pst->borderRightWidth;
                    break;
                    }
                case Ids::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = pst->borderBottomWidth;
                    break;
                    }
                case Ids::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = pst->borderLeftWidth;
                    break;
                    }
                case Ids::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = pst->borderTopLeftRadius;
                    break;
                    }
                case Ids::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = pst->borderTopRightRadius;
                    break;
                    }
                case Ids::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = pst->borderBottomRightRadius;
                    break;
                    }
                case Ids::borderBottomLeftRadius:{
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
                case Ids::borderTopWidth:{
                    auto v = fget();
                    v->borderTopWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::borderRightWidth:{
                    auto v = fget();
                    v->borderRightWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::borderBottomWidth:{
                    auto v = fget();
                    v->borderBottomWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::borderLeftWidth:{
                    auto v = fget();
                    v->borderLeftWidth = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::borderTopLeftRadius:{
                    auto v = fget();
                    v->borderTopLeftRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::borderTopRightRadius:{
                    auto v = fget();
                    v->borderTopRightRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::borderBottomRightRadius:{
                    auto v = fget();
                    v->borderBottomRightRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::borderBottomLeftRadius:{
                    auto v = fget();
                    v->borderBottomLeftRadius = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleBorder::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleBorder* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
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
            case Ids::borderTopWidth:{
                auto v = fget();
                auto prevValue = v->borderTopWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->borderTopWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderTopWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderTopWidth = OGUI::Lerp(v->borderTopWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderTopWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->borderTopWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::borderRightWidth:{
                auto v = fget();
                auto prevValue = v->borderRightWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->borderRightWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderRightWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderRightWidth = OGUI::Lerp(v->borderRightWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderRightWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->borderRightWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::borderBottomWidth:{
                auto v = fget();
                auto prevValue = v->borderBottomWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->borderBottomWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderBottomWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderBottomWidth = OGUI::Lerp(v->borderBottomWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderBottomWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->borderBottomWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::borderLeftWidth:{
                auto v = fget();
                auto prevValue = v->borderLeftWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->borderLeftWidth = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->borderLeftWidth = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->borderLeftWidth = OGUI::Lerp(v->borderLeftWidth, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->borderLeftWidth = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->borderLeftWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::borderTopLeftRadius:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
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
            case Ids::borderTopRightRadius:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
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
            case Ids::borderBottomRightRadius:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
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
            case Ids::borderBottomLeftRadius:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
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
    return damage;
}

void OGUI::StyleBorder::SetupParser()
{
    CSSParser::RegisterBorderRadius();
	{
        using namespace CSSParser;
        std::string grammar = "border-top-width <- 'border-top-width' _ ':' _ (GlobalValue / Length)";
        RegisterProperty("border-top-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopWidth;
            parser["border-top-width"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-right-width <- 'border-right-width' _ ':' _ (GlobalValue / Length)";
        RegisterProperty("border-right-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderRightWidth;
            parser["border-right-width"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-bottom-width <- 'border-bottom-width' _ ':' _ (GlobalValue / Length)";
        RegisterProperty("border-bottom-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomWidth;
            parser["border-bottom-width"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-left-width <- 'border-left-width' _ ':' _ (GlobalValue / Length)";
        RegisterProperty("border-left-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderLeftWidth;
            parser["border-left-width"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-top-left-radius <- 'border-top-left-radius' _ ':' _ (GlobalValue / YGValueZero)";
        RegisterProperty("border-top-left-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopLeftRadius;
            parser["border-top-left-radius"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-top-right-radius <- 'border-top-right-radius' _ ':' _ (GlobalValue / YGValueZero)";
        RegisterProperty("border-top-right-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopRightRadius;
            parser["border-top-right-radius"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-bottom-right-radius <- 'border-bottom-right-radius' _ ':' _ (GlobalValue / YGValueZero)";
        RegisterProperty("border-bottom-right-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomRightRadius;
            parser["border-bottom-right-radius"] = [](peg::SemanticValues& vs, std::any& dt){
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
        std::string grammar = "border-bottom-left-radius <- 'border-bottom-left-radius' _ ':' _ (GlobalValue / YGValueZero)";
        RegisterProperty("border-bottom-left-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomLeftRadius;
            parser["border-bottom-left-radius"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
}