//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/effects.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"

size_t StyleEffectsEntry = 0;

size_t OGUI::StyleEffects::GetEntry()
{
    return StyleEffectsEntry;
}

void OGUI::StyleEffects::SetEntry(size_t e)
{
    StyleEffectsEntry = e;
}

const OGUI::StyleEffects& OGUI::StyleEffects::GetDefault()
{
    struct Helper
    {
        OGUI::StyleEffects value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleEffects& OGUI::StyleEffects::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleEffects* OGUI::StyleEffects::TryGet(const ComputedStyle& style)
{
    auto& s = style.structs[StyleEffectsEntry];
    return (StyleEffects*)s.ptr.get();
}

OGUI::StyleEffects& OGUI::StyleEffects::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StyleEffectsEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StyleEffects>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(StyleEffects*)s.ptr.get();
}

void OGUI::StyleEffects::Dispose(ComputedStyle& style)
{
    style.structs[StyleEffectsEntry].ptr.reset();
}

void OGUI::StyleEffects::Initialize()
{
    opacity = 1;
}

void OGUI::StyleEffects::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleEffects* st = nullptr;
    auto& s = style.structs[StyleEffectsEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleEffects*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleEffects>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleEffects>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
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
                case Ids::opacity:{
                    auto v = fget();
                    v->opacity = 1;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::opacity:{
                    auto v = fget();
                    v->opacity = pst->opacity;
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
                case Ids::opacity:{
                    auto v = fget();
                    v->opacity = sheet.Get<float>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleEffects::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleEffects* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto& s = style.structs[StyleEffectsEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleEffects*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleEffects>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleEffects>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::opacity:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->opacity = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->opacity = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->opacity = OGUI::Lerp(v->opacity, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->opacity = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}

void OGUI::StyleEffects::SetupParser()
{
	{
        using namespace CSSParser;
        static const auto grammar = "opacityValue <- GlobalValue / Number \nopacity <- 'opacity' _ ':' _ opacityValue";
        RegisterProperty("opacity");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::opacity;
            parser["opacityValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
}