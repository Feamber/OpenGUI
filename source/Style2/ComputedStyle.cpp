
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/generated/border.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Style2/generated/effects.h"
#include "OpenGUI/Style2/generated/transition.h"
#include "gsl/span"

OGUI::StyleRegistry& GetRegistry()
{
    static OGUI::StyleRegistry instance;
    return instance;
}

const OGUI::StyleRegistry& OGUI::StyleRegistry::Get()
{
    return GetRegistry();
}

void OGUI::RegisterStyleStruct(const StyleDesc& desc)
{
    auto& registry = GetRegistry();
    registry.descriptions.push_back(desc);
}

OGUI_API size_t OGUI::NewStyleStructEntry()
{
    static size_t i = 0;
    return i++;
}

namespace OGUI::CSSParser
{
    void SetupEnumParser();
    void SetupAnimValueParser();
    void SetupMathValueParser();
    void SetupLayoutValueParser();
    void SetupDrawValueParser();
}
void OGUI::RegisterBuiltinStructs()
{
    CSSParser::SetupAnimValueParser();
    CSSParser::SetupMathValueParser();
    CSSParser::SetupLayoutValueParser();
    CSSParser::SetupEnumParser();
    CSSParser::SetupDrawValueParser();
    AnimStyle::SetupParser();
    TransitionStyle::SetupParser();
    RegisterStyleStruct<StyleText>();
    RegisterStyleStruct<StyleBackground>();
    RegisterStyleStruct<StyleBorder>();
    RegisterStyleStruct<StylePosition>();
    RegisterStyleStruct<StyleEffects>();
}

OGUI::ComputedStyle::ComputedStyle(const ComputedStyle& other)
{
    structs = other.structs;
    for(auto& s : structs)
        s.owned = false;
}

OGUI::ComputedStyle& OGUI::ComputedStyle::operator=(const ComputedStyle& other)
{
    structs = other.structs;
    for(auto& s : structs)
        s.owned = false;
    return *this;
}

OGUI::ComputedStyle OGUI::ComputedStyle::Create(const ComputedStyle *parent)
{
    OGUI::ComputedStyle result;
    if(parent)
    {
        auto& registry = GetRegistry();
        for(auto& desc : registry.descriptions)
        {
            if(desc.inherited)
            {
                auto& s = parent->structs[desc.entry]; 
                if(s.ptr)
                    result.structs[desc.entry] = {s.ptr, false};
            }
        }
    }
    return result;
}

void OGUI::ComputedStyle::ApplyProperties(const StyleSheetStorage &sheet, const gsl::span<StyleProperty> &props, const StyleMasks& override, const ComputedStyle *parent)
{
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        desc.ApplyProperties(*this, sheet, props, override, parent);
}

OGUI::RestyleDamage OGUI::ComputedStyle::ApplyAnimatedProperties(const StyleSheetStorage &sheet, const gsl::span<AnimatedProperty> &props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        damage |= desc.ApplyAnimatedProperties(*this, sheet, props, override);
    return damage;
}

OGUI::RestyleDamage OGUI::ComputedStyle::ApplyTransitionProperties(const ComputedStyle& target, const gsl::span<TransitionProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        damage |= desc.ApplyTransitionProperties(*this, target, props, override);
    return damage;
}

void OGUI::ComputedStyle::Merge(ComputedStyle& other, const StyleMasks& override)
{
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        desc.Merge(*this, other, override);
}

void OGUI::ComputedStyle::MergeId(ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        desc.MergeId(*this, other, override);
}

size_t OGUI::ComputedStyle::GetProperty(ostr::string_view name)
{
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
    {
        auto id = desc.GetProperty(name);
        if(id != -1)
            return id;
    }
    return -1;
}