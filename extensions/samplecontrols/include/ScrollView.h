#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/XmlParser/BaseXmlFactory.h"

namespace SampleControls
{
    using namespace OGUI;

    //提供滑条
    class ScrollView : public VisualElement
    {
    public:
        std::string_view GetTypeName() override { return "ScrollView"; }
        std::string_view GetFullTypeName() override { return "SampleControls::ScrollView"; }
        ScrollView();
        void InitializeChildren();
		bool _OnMouseScroll(struct PointerScrollEvent& event);
        void UpdateScrollSize() override;
        void SetScroll(Vector2f offset) override;
        bool OnMouseDown(struct PointerDownEvent& event);
        bool OnMouseMove(struct PointerMoveEvent& event);
        bool OnMouseUp(struct PointerUpEvent& event);
        VisualElement* scrollbar;
        VisualElement* slider;

        float _sliderProgress = false;
        bool _sliderPosDirty = false;
        bool _autoSliderHeight = false;

        Vector2f _dragStartPos;
        float _dragStartValue;
        bool _dragging = false;
    };

    class OGUI_API ScrollViewXmlFactory : public VisualElementXmlFactory
    {
    public:
        static const OGUI::Name& GetFullName();
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
        virtual bool OnInitElementChildPost(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent) override;
    };
}