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
    KeyDownEvent : KeyData
    {
        static const Name& GetEventName();
    };
    struct OGUI_API KeyUpEvent : KeyData
    {
        static const Name& GetEventName();
    };
    struct OGUI_API KeyHoldEvent : KeyData
    {
        static const Name& GetEventName();
    };

}