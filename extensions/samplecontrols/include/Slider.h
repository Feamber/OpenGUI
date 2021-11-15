#pragma once
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/AttributeBind.h"
#include <vector>

namespace SampleControls
{
    using namespace OGUI;
    enum class SliderDirection
    {
        Horizontal,
        Vertical
    };
    class OGUI_API Slider : public VisualElement
    {
    public:
        float _minValue = 0.f;
        float _maxValue = 1.f;
        float _value = 0.f;
        SliderDirection _direction;

        std::string_view GetTypeName() override { return "Slider"; }
        std::string_view GetFullTypeName() override { return "SampleControls::Slider"; };
        Slider();
        static void RegisterXml();
        bool OnMouseDown(struct PointerDownEvent& event);
        bool OnMouseMove(struct PointerMoveEvent& event);
        bool OnMouseUp(struct PointerUpEvent& event);
        void OnMinMaxChanged();
        void OnValueChanged();
        std::vector<VisualElement*> _animatedChildren;
        Vector2f _dragStartPos;
        float _dragStartValue;
        bool _dragging = false;
    private:
        void Initialize(VisualElement* child, std::vector<StyleSheet*>& sheets);
        void InitializeChildren();
    };
}