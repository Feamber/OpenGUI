#pragma once
#include "OpenGUI/Core/Math/Vector.h"
namespace OGUI
{
    struct TextShadow
    {
        Vector2f offset;
        float outlineSize = 0;
        float blur = 0;
        Color4f color;
    };
}