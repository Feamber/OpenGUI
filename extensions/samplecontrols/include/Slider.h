#pragma once
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/AttributeBind.h"

namespace SampleControls
{
    using namespace OGUI;
    enum class SliderDirection
    {
        Horizontal,
        Vertical
    };
    class Slider : public VisualElement
    {
    public:
        float _minValue;
        float _maxValue;
        float _value;
        SliderDirection _direction;

        Slider();
        void Initialize();
        static void RegisterXml();
        void OnMouseDown(struct PointerDownEvent& event);
        void OnMouseMove(struct PointerMoveEvent& event);
        void OnMouseUp(struct PointerUpEvent& event);
        void OnMinMaxChanged() {};
        void OnValueChanged() {};
    };
}