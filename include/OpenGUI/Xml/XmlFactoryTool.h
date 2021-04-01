#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/VisualElement.h"

namespace OGUI
{
    class TemplateContainer : public VisualElement
    {
    public:
        std::map<std::string, VisualElement*> slots;

        virtual std::string_view GetTypeName() { return "TemplateContainer"; }
        virtual std::string_view GetFullTypeName() { return "OGUI::TemplateContainer"; }
    };
}