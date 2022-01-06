#pragma once
#include "Configure.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/XmlParser/BaseXmlFactory.h"
#include "OpenGUI/Event/PointerEvent.h"

namespace SampleControls
{
    using namespace OGUI;

    //提供滑条
    class SAMPLECONTROLS_API ScrollView : public VisualElement
    {
    public:
        ostr::string_view GetTypeName() override { return u"ScrollView"; }
        ostr::string_view GetFullTypeName() override { return u"SampleControls::ScrollView"; }
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

    class SAMPLECONTROLS_API ScrollViewXmlFactory : public VisualElementXmlFactory
    {
    public:
        static const OGUI::Name& GetFullName();
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
        virtual bool OnInitElementChildPost(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent) override;
    };
}