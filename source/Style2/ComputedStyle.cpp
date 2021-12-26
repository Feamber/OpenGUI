
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Style2/Rule.h"
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

namespace OGUI
{
	//helper
	template<class T>
	StyleDesc RegisterStyleStruct()
	{
		StyleDesc desc;
		desc.hash = T::hash;
		desc.inherited = T::inherited;
		desc.name = T::name;
		desc.ApplyAnimatedProperties = &T::ApplyAnimatedProperties;
		desc.ApplyProperties = &T::ApplyProperties;
        RegisterStyleStruct(desc);
        T::SetupParser();
		return desc;
	}
}

void OGUI::RegisterBuiltinStructs()
{
    RegisterStyleStruct<StyleText>();
    RegisterStyleStruct<StyleBackground>();
    RegisterStyleStruct<StyleBorder>();
    RegisterStyleStruct<StylePosition>();
    RegisterStyleStruct<StyleEffects>();
}

OGUI::ComputedStyle::ComputedStyle(const ComputedStyle& other)
{
    for(auto& pair : other.structs)
        structs.insert({pair.first, {pair.second.ptr, false}});
}

OGUI::ComputedStyle& OGUI::ComputedStyle::operator=(const ComputedStyle& other)
{
    structs.clear();
    for(auto& pair : other.structs)
        structs.insert({pair.first, {pair.second.ptr, false}});
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
                auto iter = parent->structs.find(desc.hash); 
                if(iter != parent->structs.end())
                {
                    result.structs.insert({desc.hash, {iter->second.ptr, false}});
                }
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