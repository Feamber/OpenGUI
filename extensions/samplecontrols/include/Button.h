#pragma once
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/XmlParser/BaseXmlFactory.h"
#include "OpenGUI/Event/PointerEvent.h"

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
        bool _pressed = false;
    };

    class OGUI_API ButtonXmlFactory : public VisualElementXmlFactory
    {
    public:
        static const OGUI::Name& GetFullName();
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
    };
}