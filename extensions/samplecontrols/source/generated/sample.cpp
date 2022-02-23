//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "generated/sample.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"

size_t StyleSampleEntry = 0;

size_t OGUI::StyleSample::GetEntry()
{
    return StyleSampleEntry;
}

void OGUI::StyleSample::SetEntry(size_t e)
{
    StyleSampleEntry = e;
}

const OGUI::StyleSample& OGUI::StyleSample::GetDefault()
{
    struct Helper
    {
        OGUI::StyleSample value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleSample& OGUI::StyleSample::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleSample* OGUI::StyleSample::TryGet(const ComputedStyle& style)
{
    auto& s = style.structs[StyleSampleEntry];
    return (StyleSample*)s.ptr.get();
}

OGUI::StyleSample& OGUI::StyleSample::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StyleSampleEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StyleSample>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StyleSample>(*(OGUI::StyleSample*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(StyleSample*)s.ptr.get();
}

void OGUI::StyleSample::Dispose(ComputedStyle& style)
{
    style.structs[StyleSampleEntry].ptr.reset();
}

void OGUI::StyleSample::Initialize()
{
    someValue = 0.5f;
}

void OGUI::StyleSample::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    auto mask = override[StyleSampleEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::someValue: if(mask & (1ull<<0)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Ids::someValue: {
                    auto& v = GetOrAdd(style);
                    v.someValue = 0.5f;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::someValue:{
                    auto& v = GetOrAdd(style);
                    v.someValue = pst->someValue;
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
                case Ids::someValue:{
                    auto& v = GetOrAdd(style);
                    v.someValue = sheet.Get<float>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleSample::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleSampleEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::someValue: if(mask & (1ull<<0)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::someValue:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 0.f)
                    v.someValue = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v.someValue = sheet.Get<float>(prop.to);
                else
                    v.someValue = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}


OGUI::RestyleDamage OGUI::StyleSample::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleSampleEntry];
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::someValue: if(mask & (1ull<<0)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::someValue:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.someValue = dst.someValue;
                else
                    v.someValue = OGUI::Lerp(src.someValue, dst.someValue, prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage; 
}

void OGUI::StyleSample::Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StyleSampleEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.someValue = po->someValue;
}

void OGUI::StyleSample::MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    for(auto prop : override)
    {
        switch(prop)
        {
            case Ids::someValue: {
                 auto& v = GetOrAdd(style);
                 v.someValue = po->someValue;
                 break;
            }
        }
    }
}

size_t OGUI::StyleSample::GetProperty(ostr::string_view pname)
{
    switchstr(pname)
    {
        casestr("some-value") return Ids::someValue;
        default: return -1;
    }
    return -1;
}

void OGUI::StyleSample::SetupParser()
{
	{
        using namespace CSSParser;
        static const auto grammar = "some-valueValue <- GlobalValue / Number \nsome-value <- 'some-value' _ ':' _ some-valueValue";
        RegisterProperty("some-value");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::someValue;
            parser["some-valueValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
}


void OGUI::SetStyleSomeValue(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleSampleEntry] |= 1ull<<0;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleSample::Ids::someValue)
        {
            transition = &tran;
            break;
        }
    }
    if(transition)
    {
        StyleSample::GetOrAdd(element->_transitionDstStyle).someValue = value;
        StyleSample::GetOrAdd(element->_transitionSrcStyle).someValue = StyleSample::Get(element->_style).someValue;
        transition->time = 0.f;
    }
    else
    {
        StyleSample::GetOrAdd(element->_style).someValue = value;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleSomeValue(VisualElement* element)
{
    element->_procedureOverrides[StyleSampleEntry] &= ~(1ull<<0);
}
