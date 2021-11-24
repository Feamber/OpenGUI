#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Reflection/reflection.h"

namespace OGUI reflect
{
    struct reflect attr("event":true) 
    KeyData : public EventBase
    {
        attr("name":"gesture-type")
        EGestureEvent gestureType = EGestureEvent::None;
        attr("hide":true)
        EKeyCode key;
    };

    struct OGUI_API KeyDownEvent : KeyData
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