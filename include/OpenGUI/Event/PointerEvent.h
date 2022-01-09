#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    struct attr("event-data":true) 
    PointerData : public EventBase
    {
        int pointerId = 0;
        ostr::string_view pointerType = u"unknown";
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
    
    struct OGUI_API push_attr("event":true) 
    PointerDownEvent : PointerData {};
    struct OGUI_API PointerMoveEvent : PointerData {};
    struct OGUI_API PointerUpEvent : PointerData {};
    struct OGUI_API PointerClickEvent : PointerData {};
    struct OGUI_API PointerDoubleClickEvent : PointerData {};
    struct OGUI_API PointerEnterEvent : PointerData {};
    struct OGUI_API PointerLeaveEvent : PointerData {};
    struct OGUI_API PointerScrollEvent : PointerData{};
    struct OGUI_API TouchGestureEvent : PointerData {};
    struct OGUI_API TouchFirstMoveEvent : PointerData {};
    struct OGUI_API TouchPressureChangeEvent : PointerData {};
}