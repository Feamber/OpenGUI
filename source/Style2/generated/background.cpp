//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"

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
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StyleBackground>(*(OGUI::StyleBackground*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
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
    backgroundMaterial = {};
}

void OGUI::StyleBackground::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const gsl::span<size_t>& override, const ComputedStyle* parent)
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
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBackground>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    auto mask = override[StyleBackgroundEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::backgroundColor: if(mask & (1ull<<0)) continue; break;
            case Ids::backgroundImage: if(mask & (1ull<<1)) continue; break;
            case Ids::backgroundMaterial: if(mask & (1ull<<2)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Ids::backgroundColor: {
                    auto v = fget();
                    v->backgroundColor = Color4f(1.f,1.f,1.f,1.f);
                    break;
                    }
                case Ids::backgroundImage: {
                    auto v = fget();
                    v->backgroundImage = {};
                    break;
                    }
                case Ids::backgroundMaterial: {
                    auto v = fget();
                    v->backgroundMaterial = {};
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
                case Ids::backgroundMaterial:{
                    auto v = fget();
                    v->backgroundMaterial = pst->backgroundMaterial;
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
                    v->backgroundImage = sheet.Get<const ostr::string_view>(prop.value);
                    break;
                    }
                case Ids::backgroundMaterial:{
                    auto v = fget();
                    v->backgroundMaterial = sheet.Get<const ostr::string_view>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleBackground::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const gsl::span<size_t>& override)
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
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBackground>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    
    auto mask = override[StyleBackgroundEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::backgroundColor: if(mask & (1ull<<0)) continue; break;
            case Ids::backgroundImage: if(mask & (1ull<<1)) continue; break;
            case Ids::backgroundMaterial: if(mask & (1ull<<2)) continue; break;
        }
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
                    v->backgroundImage = sheet.Get<const ostr::string_view>(prop.from);
                else if(prop.alpha == 1.f)
                    v->backgroundImage = sheet.Get<const ostr::string_view>(prop.to);
                else if(prop.from == prop.to)
                    v->backgroundImage = OGUI::Lerp(v->backgroundImage, sheet.Get<const ostr::string_view>(prop.to), prop.alpha);
                else
                    v->backgroundImage = OGUI::Lerp(sheet.Get<const ostr::string_view>(prop.from), sheet.Get<const ostr::string_view>(prop.to), prop.alpha);
                
                break;
                }
            case Ids::backgroundMaterial:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->backgroundMaterial = sheet.Get<const ostr::string_view>(prop.from);
                else if(prop.alpha == 1.f)
                    v->backgroundMaterial = sheet.Get<const ostr::string_view>(prop.to);
                else if(prop.from == prop.to)
                    v->backgroundMaterial = OGUI::Lerp(v->backgroundMaterial, sheet.Get<const ostr::string_view>(prop.to), prop.alpha);
                else
                    v->backgroundMaterial = OGUI::Lerp(sheet.Get<const ostr::string_view>(prop.from), sheet.Get<const ostr::string_view>(prop.to), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}

void OGUI::StyleBackground::Merge(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StyleBackgroundEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.backgroundColor = po->backgroundColor;
    if(mask & (1ull << 1))
        s.backgroundImage = po->backgroundImage;
    if(mask & (1ull << 2))
        s.backgroundMaterial = po->backgroundMaterial;
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
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const ostr::string_view>(ostr::string::decode_from_utf8(std::any_cast<const std::string_view&>(vs[0])))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "background-materialValue <- GlobalValue / URL \nbackground-material <- 'background-material' _ ':' _ background-materialValue";
        RegisterProperty("background-material");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::backgroundMaterial;
            parser["background-materialValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const ostr::string_view>(ostr::string::decode_from_utf8(std::any_cast<const std::string_view&>(vs[0])))});
            };
        });
    }
}


void OGUI::SetStyleBackgroundColor(VisualElement* element, const Color4f& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<0;
    StyleBackground::GetOrAdd(element->_style).backgroundColor = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleBackgroundColor(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<0);
}
void OGUI::SetStyleBackgroundImage(VisualElement* element, const ostr::string_view& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<1;
    StyleBackground::GetOrAdd(element->_style).backgroundImage = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleBackgroundImage(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<1);
}
void OGUI::SetStyleBackgroundMaterial(VisualElement* element, const ostr::string_view& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<2;
    StyleBackground::GetOrAdd(element->_style).backgroundMaterial = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
void OGUI::ResetStyleBackgroundMaterial(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<2);
}
