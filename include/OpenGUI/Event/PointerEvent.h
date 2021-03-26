#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"

namespace OGUI
{

    struct PointerData
    {
        int pointerId;
        std::string_view pointerType;
        bool isPrimary;
        bool isTouch;
        bool isAbsolute;
        bool isDoubleClick;
        EGestureEvent gestureType;
        EMouseKey buttonPrimary;
        Vector2f position;
        Vector2f deltaPosition;
        Vector2f wheelOrGestureDelta;
        float accTime;
        int clickCount;
        float pressure;
    };
    
    struct PointerDownEvent : PointerData {};
    struct PointerMoveEvent : PointerData {};
    struct PointerUpEvent : PointerData {};
    struct PointerClickEvent : PointerData {};
    struct PointerDoubleClickEvent : PointerData {};
    struct MouseEnterEvent : PointerData {};
    struct MouseLeaveEvent : PointerData {};
    struct TouchGestureEvent : PointerData {};
    struct TouchFirstMoveEvent : PointerData {};
    struct TouchPressureChangeEvent : PointerData {};
}