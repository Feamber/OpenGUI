#pragma once
#include "OpenGUI/VisualElement.h"

namespace OGUI
{
    class OGUI_API TemplateContainer : public VisualElement
    {
        public:
        std::map<ostr::string, VisualElement*> slots;
        virtual std::string_view GetTypeName() { return "TemplateContainer"; }
        virtual std::string_view GetFullTypeName() { return "OGUI::TemplateContainer"; }
    };
}