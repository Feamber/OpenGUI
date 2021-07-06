#pragma once
#include "OpenGUI/VisualElement.h"

namespace OGUI
{
    class OGUI_API VisualWindow : public VisualElement
    {
    public:
        VisualWindow();

        virtual std::string_view GetTypeName() override { return "VisualWindow"; }
        virtual std::string_view GetFullTypeName() override { return "OGUI::VisualWindow"; }
    };
}