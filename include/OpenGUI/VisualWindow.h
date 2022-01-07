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

        virtual ostr::string_view GetTypeName() override { return u"VisualWindow"; }
        virtual ostr::string_view GetFullTypeName() override { return u"OGUI::VisualWindow"; }
    };
}