#pragma once
#include <string_view>
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    struct push_attr("event":true) 
    PointerData : public EventBase
    {
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
    
    struct OGUI_API PointerDownEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerMoveEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerUpEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerClickEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerDoubleClickEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerEnterEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerLeaveEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API PointerScrollEvent : PointerData
    {
        static const Name& GetEventName();
    };
    struct OGUI_API TouchGestureEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API TouchFirstMoveEvent : PointerData 
    {
        static const Name& GetEventName();
    };
    struct OGUI_API TouchPressureChangeEvent : PointerData 
    {
        static const Name& GetEventName();
    };
}