#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{

    struct KeyData : public EventBase
    {
        EGestureEvent gestureType = EGestureEvent::None;
        EKeyCode key;
    };

    struct OGUI_API KeyDownEvent : KeyData
    {
        static const Name& eventName();
    };
    struct OGUI_API KeyUpEvent : KeyData
    {
        static const Name& eventName();
    };
    struct OGUI_API KeyHoldEvent : KeyData
    {
        static const Name& eventName();
    };

}