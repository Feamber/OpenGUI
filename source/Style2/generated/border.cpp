//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/border.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"
#include "border_shorthands.h"

size_t StyleBorderEntry = 0;

size_t OGUI::StyleBorder::GetEntry()
{
    return StyleBorderEntry;
}

void OGUI::StyleBorder::SetEntry(size_t e)
{
    StyleBorderEntry = e;
}

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
    auto& s = style.structs[StyleBorderEntry];
    return (StyleBorder*)s.ptr.get();
}

OGUI::StyleBorder& OGUI::StyleBorder::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StyleBorderEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StyleBorder>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StyleBorder>(*(OGUI::StyleBorder*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(StyleBorder*)s.ptr.get();
}

void OGUI::StyleBorder::Dispose(ComputedStyle& style)
{
    style.structs[StyleBorderEntry].ptr.reset();
}

void OGUI::StyleBorder::Initialize()
{
    borderTopWidth = 0.f;
    borderRightWidth = 0.f;
    borderBottomWidth = 0.f;
    borderLeftWidth = 0.f;
    borderTopLeftRadius = YGValueZero;
    borderTopRightRadius = YGValueZero;
    borderBottomRightRadius = YGValueZero;
    borderBottomLeftRadius = YGValueZero;
}

void OGUI::StyleBorder::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const gsl::span<size_t>& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StyleBorder* st = nullptr;
    auto& s = style.structs[StyleBorderEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleBorder*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBorder>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBorder>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    auto mask = override[StyleBorderEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::borderTopWidth: if(mask & (1ull<<0)) continue; break;
            case Ids::borderRightWidth: if(mask & (1ull<<1)) continue; break;
            case Ids::borderBottomWidth: if(mask & (1ull<<2)) continue; break;
            case Ids::borderLeftWidth: if(mask & (1ull<<3)) continue; break;
            case Ids::borderTopLeftRadius: if(mask & (1ull<<4)) continue; break;
            case Ids::borderTopRightRadius: if(mask & (1ull<<5)) continue; break;
            case Ids::borderBottomRightRadius: if(mask & (1ull<<6)) continue; break;
            case Ids::borderBottomLeftRadius: if(mask & (1ull<<7)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Ids::borderTopWidth: {
                    auto v = fget();
                    v->borderTopWidth = 0.f;
                    break;
                    }
                case Ids::borderRightWidth: {
                    auto v = fget();
                    v->borderRightWidth = 0.f;
                    break;
                    }
                case Ids::borderBottomWidth: {
                    auto v = fget();
                    v->borderBottomWidth = 0.f;
                    break;
                    }
                case Ids::borderLeftWidth: {
                    auto v = fget();
                    v->borderLeftWidth = 0.f;
                    break;
                    }
                case Ids::borderTopLeftRadius: {
                    auto v = fget();
                    v->borderTopLeftRadius = YGValueZero;
                    break;
                    }
                case Ids::borderTopRightRadius: {
                    auto v = fget();
                    v->borderTopRightRadius = YGValueZero;
                    break;
                    }
                case Ids::borderBottomRightRadius: {
                    auto v = fget();
                    v->borderBottomRightRadius = YGValueZero;
                    break;
                    }
                case Ids::borderBottomLeftRadius: {
                    auto v = fget();
                    v->borderBottomLeftRadius = YGValueZero;
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


OGUI::RestyleDamage OGUI::StyleBorder::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const gsl::span<size_t>& override)
{
    OGUI::StyleBorder* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto& s = style.structs[StyleBorderEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StyleBorder*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StyleBorder>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StyleBorder>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    
    auto mask = override[StyleBorderEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::borderTopWidth: if(mask & (1ull<<0)) continue; break;
            case Ids::borderRightWidth: if(mask & (1ull<<1)) continue; break;
            case Ids::borderBottomWidth: if(mask & (1ull<<2)) continue; break;
            case Ids::borderLeftWidth: if(mask & (1ull<<3)) continue; break;
            case Ids::borderTopLeftRadius: if(mask & (1ull<<4)) continue; break;
            case Ids::borderTopRightRadius: if(mask & (1ull<<5)) continue; break;
            case Ids::borderBottomRightRadius: if(mask & (1ull<<6)) continue; break;
            case Ids::borderBottomLeftRadius: if(mask & (1ull<<7)) continue; break;
        }
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

void OGUI::StyleBorder::Merge(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StyleBorderEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.borderTopWidth = po->borderTopWidth;
    if(mask & (1ull << 1))
        s.borderRightWidth = po->borderRightWidth;
    if(mask & (1ull << 2))
        s.borderBottomWidth = po->borderBottomWidth;
    if(mask & (1ull << 3))
        s.borderLeftWidth = po->borderLeftWidth;
    if(mask & (1ull << 4))
        s.borderTopLeftRadius = po->borderTopLeftRadius;
    if(mask & (1ull << 5))
        s.borderTopRightRadius = po->borderTopRightRadius;
    if(mask & (1ull << 6))
        s.borderBottomRightRadius = po->borderBottomRightRadius;
    if(mask & (1ull << 7))
        s.borderBottomLeftRadius = po->borderBottomLeftRadius;
}

void OGUI::StyleBorder::SetupParser()
{
    CSSParser::RegisterBorderRadius();
	{
        using namespace CSSParser;
        static const auto grammar = "border-top-widthValue <- GlobalValue / Length \nborder-top-width <- 'border-top-width' _ ':' _ border-top-widthValue";
        RegisterProperty("border-top-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopWidth;
            parser["border-top-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-right-widthValue <- GlobalValue / Length \nborder-right-width <- 'border-right-width' _ ':' _ border-right-widthValue";
        RegisterProperty("border-right-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderRightWidth;
            parser["border-right-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-bottom-widthValue <- GlobalValue / Length \nborder-bottom-width <- 'border-bottom-width' _ ':' _ border-bottom-widthValue";
        RegisterProperty("border-bottom-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomWidth;
            parser["border-bottom-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-left-widthValue <- GlobalValue / Length \nborder-left-width <- 'border-left-width' _ ':' _ border-left-widthValue";
        RegisterProperty("border-left-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderLeftWidth;
            parser["border-left-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-top-left-radiusValue <- GlobalValue / LengthPercentage \nborder-top-left-radius <- 'border-top-left-radius' _ ':' _ border-top-left-radiusValue";
        RegisterProperty("border-top-left-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopLeftRadius;
            parser["border-top-left-radiusValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-top-right-radiusValue <- GlobalValue / LengthPercentage \nborder-top-right-radius <- 'border-top-right-radius' _ ':' _ border-top-right-radiusValue";
        RegisterProperty("border-top-right-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderTopRightRadius;
            parser["border-top-right-radiusValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-bottom-right-radiusValue <- GlobalValue / LengthPercentage \nborder-bottom-right-radius <- 'border-bottom-right-radius' _ ':' _ border-bottom-right-radiusValue";
        RegisterProperty("border-bottom-right-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomRightRadius;
            parser["border-bottom-right-radiusValue"] = [](peg::SemanticValues& vs, std::any& dt){
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
        static const auto grammar = "border-bottom-left-radiusValue <- GlobalValue / LengthPercentage \nborder-bottom-left-radius <- 'border-bottom-left-radius' _ ':' _ border-bottom-left-radiusValue";
        RegisterProperty("border-bottom-left-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::borderBottomLeftRadius;
            parser["border-bottom-left-radiusValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
}


attr("script": true)
void OGUI::SetStyleBorderTopWidth(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<0;
    StyleBorder::GetOrAdd(element->_style).borderTopWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderTopWidth(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<0);
}
attr("script": true)
void OGUI::SetStyleBorderRightWidth(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<1;
    StyleBorder::GetOrAdd(element->_style).borderRightWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderRightWidth(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<1);
}
attr("script": true)
void OGUI::SetStyleBorderBottomWidth(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<2;
    StyleBorder::GetOrAdd(element->_style).borderBottomWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderBottomWidth(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<2);
}
attr("script": true)
void OGUI::SetStyleBorderLeftWidth(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<3;
    StyleBorder::GetOrAdd(element->_style).borderLeftWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderLeftWidth(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<3);
}
attr("script": true)
void OGUI::SetStyleBorderTopLeftRadius(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<4;
    StyleBorder::GetOrAdd(element->_style).borderTopLeftRadius = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderTopLeftRadius(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<4);
}
attr("script": true)
void OGUI::SetStyleBorderTopRightRadius(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<5;
    StyleBorder::GetOrAdd(element->_style).borderTopRightRadius = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderTopRightRadius(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<5);
}
attr("script": true)
void OGUI::SetStyleBorderBottomRightRadius(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<6;
    StyleBorder::GetOrAdd(element->_style).borderBottomRightRadius = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderBottomRightRadius(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<6);
}
attr("script": true)
void OGUI::SetStyleBorderBottomLeftRadius(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StyleBorderEntry] |= 1ull<<7;
    StyleBorder::GetOrAdd(element->_style).borderBottomLeftRadius = value;
    RestyleDamage damage = RestyleDamage::None;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBorderBottomLeftRadius(VisualElement* element)
{
    element->_procedureOverrides[StyleBorderEntry] &= ~(1ull<<7);
}
