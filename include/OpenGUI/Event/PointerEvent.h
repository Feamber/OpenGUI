#pragma once
#include <string_view>
#include "OpenGUI/Core/Math.h"

namespace OGUI
{
    enum class GestureEvent : uint8
    {
        None,
        Scroll,
        Magnify,
        Swipe,
        Rotate,
        LongPress,
        Count
    };

    enum class ButtonId : uint8
    {
        Left,
        Middle,
        Right,
        Ex1,
        Ex2,
        Ex3,
        Ex4
    };

    struct PointerData
    {
        int pointerId;
        std::string_view pointerType;
        bool isPrimary;
        bool isTouch;
        GestureEvent gestureType;
        int buttonMask;
        ButtonId buttonPrimary;
        Vector2f position;
        Vector2f deltaPosition;
        Vector2f wheelOrGestureDelta;
        float deltaTime;
        int clickCount;
        float pressure;
    };
    
    struct PointerDownEvent : PointerData {};
    struct PointerMoveEvent : PointerData {};
    struct PointerUpEvent : PointerData {};
    struct PointerClickEvent : PointerData {};
    struct MouseEnterEvent : PointerData {};
    struct MouseLeaveEvent : PointerData {};
    struct TouchGestureEvent : PointerData {};
    struct TouchFirstMoveEvent : PointerData {};
    struct TouchPressureChangeEvent : PointerData {};
}