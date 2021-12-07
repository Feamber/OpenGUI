
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/PointerEvent.h"

namespace OGUI
{
    const Name& PreGotKeyboardFocusEvent::GetEventName() 
    {
        static Name name = "pre-got-keyboard-focus";
        return name;
    };
    const Name& PreLostKeyboardFocusEvent::GetEventName()
    {
        static Name name = "pre-lost-keyboard-focus";
        return name;
    };
    const Name& PreGotFocusEvent::GetEventName()
    {
        static Name name = "pre-got-focus";
        return name;
    };
    const Name& PreLostFocusEvent::GetEventName()
    {
        static Name name = "pre-lost-focus";
        return name;
    };
    const Name& GotKeyboardFocusEvent::GetEventName()
    {
        static Name name = "got-keyboard-focus";
        return name;
    };
    const Name& LostKeyboardFocusEvent::GetEventName()
    {
        static Name name = "lost-keyboard-focus";
        return name;
    };
    const Name& GotFocusEvent::GetEventName()
    {
        static Name name = "got-focus";
        return name;
    };
    const Name& LostFocusEvent::GetEventName()
    {
        static Name name = "lost-focus";
        return name;
    };
    
    const Name& KeyDownEvent::GetEventName()
    {
        static Name name = "key-down";
        return name;
    };
    const Name& KeyUpEvent::GetEventName()
    {
        static Name name = "key-up";
        return name;
    };
    const Name& KeyHoldEvent::GetEventName()
    {
        static Name name = "key-hold";
        return name;
    };

    const Name& PointerDownEvent::GetEventName()
    {
        static Name name = "pointer-down";
        return name;
    };
    const Name& PointerMoveEvent::GetEventName()
    {
        static Name name = "pointer-move";
        return name;
    };
    const Name& PointerUpEvent::GetEventName()
    {
        static Name name = "pointer-up";
        return name;
    };
    const Name& PointerClickEvent::GetEventName()
    {
        static Name name = "pointer-click";
        return name;
    };
    const Name& PointerDoubleClickEvent::GetEventName()
    {
        static Name name = "pointer-double-click";
        return name;
    };
    const Name& PointerEnterEvent::GetEventName()
    {
        static Name name = "pointer-enter";
        return name;
    };
    const Name& PointerLeaveEvent::GetEventName()
    {
        static Name name = "pointer-leave";
        return name;
    };
    const Name& PointerScrollEvent::GetEventName()
    {
        static Name name = "pointer-scroll";
        return name;
    };
    const Name& TouchGestureEvent::GetEventName()
    {
        static Name name = "touch-gesture";
        return name;
    };
    const Name& TouchFirstMoveEvent::GetEventName()
    {
        static Name name = "touch-first-move";
        return name;
    };
    const Name& TouchPressureChangeEvent::GetEventName()
    {
        static Name name = "touch-pressure-change";
        return name;
    };
}