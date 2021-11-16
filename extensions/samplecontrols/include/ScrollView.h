#pragma once
#include "OpenGUI/VisualElement.h"

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
        static void RegisterXml();
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
}