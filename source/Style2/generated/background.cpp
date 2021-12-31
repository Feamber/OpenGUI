//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"

size_t StyleBackgroundEntry = 0;

size_t OGUI::StyleBackground::GetEntry()
{
    return StyleBackgroundEntry;
}

void OGUI::StyleBackground::SetEntry(size_t e)
{
    StyleBackgroundEntry = e;
}

const OGUI::StyleBackground& OGUI::StyleBackground::GetDefault()
{
    struct Helper
    {
        OGUI::StyleBackground value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StyleBackground& OGUI::StyleBackground::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StyleBackground* OGUI::StyleBackground::TryGet(const ComputedStyle& style)
{
    auto& s = style.structs[StyleBackgroundEntry];
    return (StyleBackground*)s.ptr.get();
}

OGUI::StyleBackground& OGUI::StyleBackground::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StyleBackgroundEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StyleBackground>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        return *value.get();
    }
    else 
        return *(StyleBackground*)s.ptr.get();
}

void OGUI::StyleBackground::Dispose(ComputedStyle& style)
{
    style.structs[StyleBackgroundEntry].ptr.reset();
}

void OGUI::StyleBackground::Initialize()
{
    backgroundColor = Color4f(1.f,1.f,1.f,1.f);
    backgroundImage = {};
}

void OGUI::StyleBackground::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleBackground* st = nullptr;
    auto& s = style.structs[StyleBackgroundEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleBackground*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBackground>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBackground>(*st);
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
                case Ids::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = Color4f(1.f,1.f,1.f,1.f);
                    break;
                    }
                case Ids::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = {};
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = pst->backgroundColor;
                    break;
                    }
                case Ids::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = pst->backgroundImage;
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
                case Ids::backgroundColor:{
                    auto v = fget();
                    v->backgroundColor = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Ids::backgroundImage:{
                    auto v = fget();
                    v->backgroundImage = sheet.Get<const std::string_view>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleBackground::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StyleBackground* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto& s = style.structs[StyleBackgroundEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleBackground*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBackground>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBackground>(*st);
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
            case Ids::backgroundColor:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->backgroundColor = sheet.Get<Color4f>(prop.from);
                else if(prop.alpha == 1.f)
                    v->backgroundColor = sheet.Get<Color4f>(prop.to);
                else if(prop.from == prop.to)
                    v->backgroundColor = OGUI::Lerp(v->backgroundColor, sheet.Get<Color4f>(prop.to), prop.alpha);
                else
                    v->backgroundColor = OGUI::Lerp(sheet.Get<Color4f>(prop.from), sheet.Get<Color4f>(prop.to), prop.alpha);
                
                break;
                }
            case Ids::backgroundImage:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->backgroundImage = sheet.Get<const std::string_view>(prop.from);
                else if(prop.alpha == 1.f)
                    v->backgroundImage = sheet.Get<const std::string_view>(prop.to);
                else if(prop.from == prop.to)
                    v->backgroundImage = OGUI::Lerp(v->backgroundImage, sheet.Get<const std::string_view>(prop.to), prop.alpha);
                else
                    v->backgroundImage = OGUI::Lerp(sheet.Get<const std::string_view>(prop.from), sheet.Get<const std::string_view>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}

void OGUI::StyleBackground::SetupParser()
{
	{
        using namespace CSSParser;
        static const auto grammar = "background-colorValue <- GlobalValue / Color \nbackground-color <- 'background-color' _ ':' _ background-colorValue";
        RegisterProperty("background-color");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::backgroundColor;
            parser["background-colorValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "background-imageValue <- GlobalValue / URL \nbackground-image <- 'background-image' _ ':' _ background-imageValue";
        RegisterProperty("background-image");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::backgroundImage;
            parser["background-imageValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const std::string_view>(std::any_cast<const std::string_view&>(vs[0]))});
            };
        });
    }
}