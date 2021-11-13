#pragma once
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/VisualElement.h"

namespace OGUI
{
    class OGUI_API VisualWindow : public VisualElement
    {
    public:
        VisualWindow();

        WindowHandle handle;

        virtual std::string_view GetTypeName() override { return "VisualWindow"; }
        virtual std::string_view GetFullTypeName() override { return "OGUI::VisualWindow"; }
    };
}