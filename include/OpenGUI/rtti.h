#pragma once
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/rtti.generated.h"

namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::Vector2f> { static const Type* Get(); };
}