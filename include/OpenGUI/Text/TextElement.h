#ifndef UE4Runtime
#pragma once

//https://www.w3.org/TR/2021/CRD-css-text-3-20210316
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{
    class TextElement : public VisualElement
    {
        TextElement();
        ~TextElement();
        std::vector<ostr::string> _texts;
    };
}
#endif