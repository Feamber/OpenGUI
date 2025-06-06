//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/effects.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"

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
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StyleEffects>(*(OGUI::StyleEffects*)s.ptr.get());
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

void OGUI::StyleEffects::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    auto mask = override[StyleEffectsEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::opacity: if(mask & (1ull<<0)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Ids::opacity: {
                    auto& v = GetOrAdd(style);
                    v.opacity = 1;
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
                    auto& v = GetOrAdd(style);
                    v.opacity = pst->opacity;
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
                    auto& v = GetOrAdd(style);
                    v.opacity = sheet.Get<float>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleEffects::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleEffectsEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::opacity: if(mask & (1ull<<0)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::opacity:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> float
                {
                    if(!keyword)
                        return sheet.Get<float>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    || handle.index == (int)StyleKeyword::Unset
                    )
                    {
                        return GetDefault().opacity;
                    }
                    else
                    { 
                        return GetDefault().opacity;
                    }
                };
                if(prop.alpha == 0.f)
                    v.opacity = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.opacity = getValue(prop.to, prop.toKeyword);
                else
                    v.opacity = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}


OGUI::RestyleDamage OGUI::StyleEffects::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::opacity:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.opacity = dst.opacity;
                else
                    v.opacity = OGUI::Lerp(src.opacity, dst.opacity, prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage; 
}

void OGUI::StyleEffects::Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StyleEffectsEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.opacity = po->opacity;
}

void OGUI::StyleEffects::MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    for(auto prop : override)
    {
        switch(prop)
        {
            case Ids::opacity: {
                 auto& v = GetOrAdd(style);
                 v.opacity = po->opacity;
                 break;
            }
        }
    }
}

size_t OGUI::StyleEffects::GetProperty(ostr::string_view pname)
{
    switchstr(pname)
    {
        casestr("opacity") return Ids::opacity;
        default: return -1;
    }
    return -1;
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


void OGUI::SetStyleOpacity(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleEffectsEntry] |= 1ull<<0;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleEffects::Ids::opacity)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleEffects::GetOrAdd(element->_overrideStyle);
    override.opacity = value;
    if(transition)
    {
        StyleEffects::GetOrAdd(element->_transitionDstStyle).opacity = override.opacity;
        StyleEffects::GetOrAdd(element->_transitionSrcStyle).opacity = StyleEffects::Get(element->_style).opacity;
        transition->time = 0.f;
    }
    else
    {
        StyleEffects::GetOrAdd(element->_style).opacity = override.opacity;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleOpacity(VisualElement* element)
{
    element->_procedureOverrides[StyleEffectsEntry] &= ~(1ull<<0);
}
