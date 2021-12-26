#pragma once
#include "OpenGUI/meta.h"
#include <cinttypes>

namespace OGUI reflect
{
    enum class attr("style-enum":true)
    ETextStyle
    {
        Normal,
        Italic
    };

    enum class attr("style-enum":true)
    ETextAlign
    {
        Start,
        End,
        Left,
        Right,
        Center,
        Justify
    };
}