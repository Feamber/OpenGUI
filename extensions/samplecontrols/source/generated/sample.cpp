//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "generated/sample.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"

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

void OGUI::StyleSample::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleSample* st = nullptr;
    auto& s = style.structs[StyleSampleEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleSample*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleSample>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleSample>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
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
                case Ids::someValue:{
                    auto v = fget();
                    v->someValue = 0.5f;
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
                    auto v = fget();
                    v->someValue = pst->someValue;
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
                    auto v = fget();
                    v->someValue = sheet.Get<float>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleSample::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleSample* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto& s = style.structs[StyleSampleEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleSample*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleSample>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleSample>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::someValue:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->someValue = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->someValue = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->someValue = OGUI::Lerp(v->someValue, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->someValue = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
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