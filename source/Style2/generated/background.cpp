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
    backgroundGamma = false;
}

void OGUI::StyleBackground::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    auto mask = override[StyleBackgroundEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::backgroundColor: if(mask & (1ull<<0)) continue; break;
            case Ids::backgroundImage: if(mask & (1ull<<1)) continue; break;
            case Ids::backgroundMaterial: if(mask & (1ull<<2)) continue; break;
            case Ids::backgroundGamma: if(mask & (1ull<<3)) continue; break;
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
                    auto& v = GetOrAdd(style);
                    v.backgroundColor = Color4f(1.f,1.f,1.f,1.f);
                    break;
                    }
                case Ids::backgroundImage: {
                    auto& v = GetOrAdd(style);
                    v.backgroundImage = {};
                    break;
                    }
                case Ids::backgroundMaterial: {
                    auto& v = GetOrAdd(style);
                    v.backgroundMaterial = {};
                    break;
                    }
                case Ids::backgroundGamma: {
                    auto& v = GetOrAdd(style);
                    v.backgroundGamma = false;
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
                    auto& v = GetOrAdd(style);
                    v.backgroundColor = pst->backgroundColor;
                    break;
                    }
                case Ids::backgroundImage:{
                    auto& v = GetOrAdd(style);
                    v.backgroundImage = pst->backgroundImage;
                    break;
                    }
                case Ids::backgroundMaterial:{
                    auto& v = GetOrAdd(style);
                    v.backgroundMaterial = pst->backgroundMaterial;
                    break;
                    }
                case Ids::backgroundGamma:{
                    auto& v = GetOrAdd(style);
                    v.backgroundGamma = pst->backgroundGamma;
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
                    auto& v = GetOrAdd(style);
                    v.backgroundColor = sheet.Get<Color4f>(prop.value);
                    break;
                    }
                case Ids::backgroundImage:{
                    auto& v = GetOrAdd(style);
                    v.backgroundImage = sheet.Get<const ostr::string_view>(prop.value);
                    break;
                    }
                case Ids::backgroundMaterial:{
                    auto& v = GetOrAdd(style);
                    v.backgroundMaterial = sheet.Get<const ostr::string_view>(prop.value);
                    break;
                    }
                case Ids::backgroundGamma:{
                    auto& v = GetOrAdd(style);
                    v.backgroundGamma = sheet.Get<bool>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StyleBackground::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[StyleBackgroundEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::backgroundColor: if(mask & (1ull<<0)) continue; break;
            case Ids::backgroundImage: if(mask & (1ull<<1)) continue; break;
            case Ids::backgroundMaterial: if(mask & (1ull<<2)) continue; break;
            case Ids::backgroundGamma: if(mask & (1ull<<3)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::backgroundColor:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> Color4f
                {
                    if(!keyword)
                        return sheet.Get<Color4f>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    || handle.index == (int)StyleKeyword::Unset
                    )
                    {
                        return GetDefault().backgroundColor;
                    }
                    else
                    { 
                        return GetDefault().backgroundColor;
                    }
                };
                if(prop.alpha == 0.f)
                    v.backgroundColor = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.backgroundColor = getValue(prop.to, prop.toKeyword);
                else
                    v.backgroundColor = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::backgroundImage:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> const ostr::string_view
                {
                    if(!keyword)
                        return sheet.Get<const ostr::string_view>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    || handle.index == (int)StyleKeyword::Unset
                    )
                    {
                        return GetDefault().backgroundImage;
                    }
                    else
                    { 
                        return GetDefault().backgroundImage;
                    }
                };
                if(prop.alpha == 0.f)
                    v.backgroundImage = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.backgroundImage = getValue(prop.to, prop.toKeyword);
                else
                    v.backgroundImage = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::backgroundMaterial:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> const ostr::string_view
                {
                    if(!keyword)
                        return sheet.Get<const ostr::string_view>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    || handle.index == (int)StyleKeyword::Unset
                    )
                    {
                        return GetDefault().backgroundMaterial;
                    }
                    else
                    { 
                        return GetDefault().backgroundMaterial;
                    }
                };
                if(prop.alpha == 0.f)
                    v.backgroundMaterial = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.backgroundMaterial = getValue(prop.to, prop.toKeyword);
                else
                    v.backgroundMaterial = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            case Ids::backgroundGamma:{
                auto& v = GetOrAdd(style);
                auto getValue = [&](VariantHandle& handle, bool keyword) -> bool
                {
                    if(!keyword)
                        return sheet.Get<bool>(handle); 
                    if (handle.index == (int)StyleKeyword::Initial // || !pst
                    || handle.index == (int)StyleKeyword::Unset
                    )
                    {
                        return GetDefault().backgroundGamma;
                    }
                    else
                    { 
                        return GetDefault().backgroundGamma;
                    }
                };
                if(prop.alpha == 0.f)
                    v.backgroundGamma = getValue(prop.from, prop.fromKeyword);
                else if(prop.alpha == 1.f)
                    v.backgroundGamma = getValue(prop.to, prop.toKeyword);
                else
                    v.backgroundGamma = OGUI::Lerp(getValue(prop.from, prop.fromKeyword), getValue(prop.to, prop.toKeyword), prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage;
}


OGUI::RestyleDamage OGUI::StyleBackground::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::backgroundColor:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.backgroundColor = dst.backgroundColor;
                else
                    v.backgroundColor = OGUI::Lerp(src.backgroundColor, dst.backgroundColor, prop.alpha);
                
                break;
                }
            case Ids::backgroundImage:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.backgroundImage = dst.backgroundImage;
                else
                    v.backgroundImage = OGUI::Lerp(src.backgroundImage, dst.backgroundImage, prop.alpha);
                
                break;
                }
            case Ids::backgroundMaterial:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.backgroundMaterial = dst.backgroundMaterial;
                else
                    v.backgroundMaterial = OGUI::Lerp(src.backgroundMaterial, dst.backgroundMaterial, prop.alpha);
                
                break;
                }
            case Ids::backgroundGamma:{
                auto& v = GetOrAdd(style);
                if(prop.alpha == 1.f)
                    v.backgroundGamma = dst.backgroundGamma;
                else
                    v.backgroundGamma = OGUI::Lerp(src.backgroundGamma, dst.backgroundGamma, prop.alpha);
                
                break;
                }
            default: break;
        }
    }
    return damage; 
}

void OGUI::StyleBackground::Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override)
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
    if(mask & (1ull << 3))
        s.backgroundGamma = po->backgroundGamma;
}

void OGUI::StyleBackground::MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    for(auto prop : override)
    {
        switch(prop)
        {
            case Ids::backgroundColor: {
                 auto& v = GetOrAdd(style);
                 v.backgroundColor = po->backgroundColor;
                 break;
            }
            case Ids::backgroundImage: {
                 auto& v = GetOrAdd(style);
                 v.backgroundImage = po->backgroundImage;
                 break;
            }
            case Ids::backgroundMaterial: {
                 auto& v = GetOrAdd(style);
                 v.backgroundMaterial = po->backgroundMaterial;
                 break;
            }
            case Ids::backgroundGamma: {
                 auto& v = GetOrAdd(style);
                 v.backgroundGamma = po->backgroundGamma;
                 break;
            }
        }
    }
}

size_t OGUI::StyleBackground::GetProperty(ostr::string_view pname)
{
    switchstr(pname)
    {
        casestr("background-color") return Ids::backgroundColor;
        casestr("background-image") return Ids::backgroundImage;
        casestr("background-material") return Ids::backgroundMaterial;
        casestr("background-gamma") return Ids::backgroundGamma;
        default: return -1;
    }
    return -1;
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
	{
        using namespace CSSParser;
        static const auto grammar = "background-gammaValue <- GlobalValue / Bool \nbackground-gamma <- 'background-gamma' _ ':' _ background-gammaValue";
        RegisterProperty("background-gamma");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::backgroundGamma;
            parser["background-gammaValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<bool>(std::any_cast<bool&>(vs[0]))});
            };
        });
    }
}


void OGUI::SetStyleBackgroundColor(VisualElement* element, const Color4f& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<0;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleBackground::Ids::backgroundColor)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleBackground::GetOrAdd(element->_overrideStyle);
    override.backgroundColor = value;
    if(transition)
    {
        StyleBackground::GetOrAdd(element->_transitionDstStyle).backgroundColor = override.backgroundColor;
        StyleBackground::GetOrAdd(element->_transitionSrcStyle).backgroundColor = StyleBackground::Get(element->_style).backgroundColor;
        transition->time = 0.f;
    }
    else
    {
        StyleBackground::GetOrAdd(element->_style).backgroundColor = override.backgroundColor;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleBackgroundColor(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<0);
}
void OGUI::SetStyleBackgroundImage(VisualElement* element, const ostr::string_view& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<1;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleBackground::Ids::backgroundImage)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleBackground::GetOrAdd(element->_overrideStyle);
    override.backgroundImage = value;
    if(transition)
    {
        StyleBackground::GetOrAdd(element->_transitionDstStyle).backgroundImage = override.backgroundImage;
        StyleBackground::GetOrAdd(element->_transitionSrcStyle).backgroundImage = StyleBackground::Get(element->_style).backgroundImage;
        transition->time = 0.f;
    }
    else
    {
        StyleBackground::GetOrAdd(element->_style).backgroundImage = override.backgroundImage;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleBackgroundImage(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<1);
}
void OGUI::SetStyleBackgroundMaterial(VisualElement* element, const ostr::string_view& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<2;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleBackground::Ids::backgroundMaterial)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleBackground::GetOrAdd(element->_overrideStyle);
    override.backgroundMaterial = value;
    if(transition)
    {
        StyleBackground::GetOrAdd(element->_transitionDstStyle).backgroundMaterial = override.backgroundMaterial;
        StyleBackground::GetOrAdd(element->_transitionSrcStyle).backgroundMaterial = StyleBackground::Get(element->_style).backgroundMaterial;
        transition->time = 0.f;
    }
    else
    {
        StyleBackground::GetOrAdd(element->_style).backgroundMaterial = override.backgroundMaterial;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleBackgroundMaterial(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<2);
}
void OGUI::SetStyleBackgroundGamma(VisualElement* element, const bool& value)
{
    element->_procedureOverrides[StyleBackgroundEntry] |= 1ull<<3;
    ComputedTransition* transition = nullptr;
    for(auto& tran : element->_trans)
    {
        if(tran.style.transitionProperty == StyleBackground::Ids::backgroundGamma)
        {
            transition = &tran;
            break;
        }
    }
    auto& override = StyleBackground::GetOrAdd(element->_overrideStyle);
    override.backgroundGamma = value;
    if(transition)
    {
        StyleBackground::GetOrAdd(element->_transitionDstStyle).backgroundGamma = override.backgroundGamma;
        StyleBackground::GetOrAdd(element->_transitionSrcStyle).backgroundGamma = StyleBackground::Get(element->_style).backgroundGamma;
        transition->time = 0.f;
    }
    else
    {
        StyleBackground::GetOrAdd(element->_style).backgroundGamma = override.backgroundGamma;
        RestyleDamage damage = RestyleDamage::None;
        element->UpdateStyle(damage);
    }
}
void OGUI::ResetStyleBackgroundGamma(VisualElement* element)
{
    element->_procedureOverrides[StyleBackgroundEntry] &= ~(1ull<<3);
}
