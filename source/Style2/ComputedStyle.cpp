
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/generated/border.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Style2/generated/effects.h"

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

void OGUI::ComputedStyle::ApplyProperties(const StyleSheetStorage &sheet, const gsl::span<StyleProperty> &props, const ComputedStyle *parent)
{
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        desc.ApplyProperties(*this, sheet, props, parent);
}

OGUI::RestyleDamage OGUI::ComputedStyle::ApplyAnimatedProperties(const StyleSheetStorage &sheet, const gsl::span<AnimatedProperty> &props)
{
    RestyleDamage damage = RestyleDamage::None;
    auto& registry = GetRegistry();
    for(auto& desc : registry.descriptions)
        damage |= desc.ApplyAnimatedProperties(*this, sheet, props);
    return damage;
}