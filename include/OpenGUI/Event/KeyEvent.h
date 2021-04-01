#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
//#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{

    struct KeyData
    {
        EGestureEvent gestureType = EGestureEvent::None;
        EKeyCode key;
        bool pressed = false;
        bool released = false;
        bool hold = false;
    };

    struct KeyDownEvent : KeyData
    {
        //EventRoutePhase currentPhase = EventRoutePhase::None;
        //static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };
    struct KeyUpEvent : KeyData
    {
        //EventRoutePhase currentPhase = EventRoutePhase::None;
        //static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };
    struct KeyHoldEvent : KeyData
    {
        //EventRoutePhase currentPhase = EventRoutePhase::None;
        //static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

}