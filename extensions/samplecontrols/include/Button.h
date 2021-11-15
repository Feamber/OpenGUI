#pragma once
#include "OpenGUI/VisualElement.h"

namespace SampleControls
{
    using namespace OGUI;
    class Button : public VisualElement
    {
    public:
        std::string_view GetTypeName() override { return "Button"; }
        std::string_view GetFullTypeName() override { return "SampleControls::Button"; };
        Button();
        bool Active();
        bool OnPointerDown(PointerDownEvent& event);
        bool OnPointerMove(PointerMoveEvent& event);
        bool OnPointerUp(PointerUpEvent& event);
        virtual void OnClicked(){}
        static void RegisterXml();
        bool _pressed = false;
    };
}