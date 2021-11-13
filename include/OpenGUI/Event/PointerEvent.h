#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{

    struct PointerData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
        int pointerId = 0;
        std::string_view pointerType = "unknown";
        bool isPrimary = true;
        bool isTouch = false;
        bool isAbsolute = true;
        bool isDoubleClick = false;
        EGestureEvent gestureType = EGestureEvent::None;
        EMouseKey button;
        Vector2f position = Vector2f::vector_zero();
        Vector2f deltaPosition = Vector2f::vector_zero();
        Vector2f wheelOrGestureDelta= Vector2f::vector_zero();
        float accTime = 0;
        int clickCount = 1;
        float pressure = 0;
    };
    
    struct PointerDownEvent : PointerData 
    {
    };
    struct PointerMoveEvent : PointerData {};
    struct PointerUpEvent : PointerData 
    {
    };
    struct PointerClickEvent : PointerData {};
    struct PointerDoubleClickEvent : PointerData {};
    struct MouseEnterEvent : PointerData 
    {
    };
    struct MouseLeaveEvent : PointerData 
    {
    };
    struct TouchGestureEvent : PointerData {};
    struct TouchFirstMoveEvent : PointerData {};
    struct TouchPressureChangeEvent : PointerData {};
}