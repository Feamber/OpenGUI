#pragma once
#include "OpenGUI/Configure.h"
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

    enum class attr("style-enum":true)
    ETextJustify
    {
        InterWord,
        InterIdeograph
    };

    enum class 
    ETextDecorationLine
    {
        None = 0,
        Underline = 0x1,
        Overline = 0x2,
        LineThrough = 0x4,
    };

    ENUM_CLASS_FLAGS(ETextDecorationLine)
}