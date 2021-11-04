#include "OpenGUI/Style2/ComputedStyle.h"

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