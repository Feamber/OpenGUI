#pragma once
#include "OpenGUI/VisualElement.h"

namespace SampleControls
{
    class Slider : public OGUI::VisualElement
    {
    public:
        float _minValue;
        float _maxValue;
        
    };
}