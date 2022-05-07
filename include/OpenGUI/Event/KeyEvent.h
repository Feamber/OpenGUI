#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    struct attr("event-data":true) 
    KeyData : public EventBase
    {
        EGestureEvent gestureType = EGestureEvent::None;
        EKeyCode key;
    };

    struct OGUI_API push_attr("event":true) 
    KeyDownEvent : KeyData {};
    struct OGUI_API KeyUpEvent : KeyData {};
    struct OGUI_API KeyHoldEvent : KeyData {};
    struct OGUI_API KeyAnalogInputEvent : KeyData
    {
        float analogValue = 0.f;
    };
    struct OGUI_API KeyDownNavigationPostEvent : KeyData {};
}