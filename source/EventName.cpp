#define DLL_IMPLEMENTATION
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/PointerEvent.h"

namespace OGUI
{
    const Name& PreGotKeyboardFocusEvent::eventName() 
    {
        static Name name = "pre-got-keyboard-focus";
        return name;
    };
    const Name& PreLostKeyboardFocusEvent::eventName()
    {
        static Name name = "pre-lost-keyboard-focus";
        return name;
    };
    const Name& PreGotFocusEvent::eventName()
    {
        static Name name = "pre-got-focus";
        return name;
    };
    const Name& PreLostFocusEvent::eventName()
    {
        static Name name = "pre-lost-focus";
        return name;
    };
    const Name& GotKeyboardFocusEvent::eventName()
    {
        static Name name = "got-keyboard-focus";
        return name;
    };
    const Name& LostKeyboardFocusEvent::eventName()
    {
        static Name name = "lost-keyboard-focus";
        return name;
    };
    const Name& GotFocusEvent::eventName()
    {
        static Name name = "got-focus";
        return name;
    };
    const Name& LostFocusEvent::eventName()
    {
        static Name name = "lost-focus";
        return name;
    };
    
    const Name& KeyDownEvent::eventName()
    {
        static Name name = "key-down";
        return name;
    };
    const Name& KeyUpEvent::eventName()
    {
        static Name name = "key-up";
        return name;
    };
    const Name& KeyHoldEvent::eventName()
    {
        static Name name = "key-hold";
        return name;
    };

    const Name& PointerDownEvent::eventName()
    {
        static Name name = "pointer-down";
        return name;
    };
    const Name& PointerMoveEvent::eventName()
    {
        static Name name = "pointer-move";
        return name;
    };
    const Name& PointerUpEvent::eventName()
    {
        static Name name = "pointer-up";
        return name;
    };
    const Name& PointerClickEvent::eventName()
    {
        static Name name = "pointer-click";
        return name;
    };
    const Name& PointerDoubleClickEvent::eventName()
    {
        static Name name = "pointer-double-click";
        return name;
    };
    const Name& MouseEnterEvent::eventName()
    {
        static Name name = "mouse-enter";
        return name;
    };
    const Name& MouseLeaveEvent::eventName()
    {
        static Name name = "mouse-leave";
        return name;
    };
    const Name& TouchGestureEvent::eventName()
    {
        static Name name = "touch-gesture";
        return name;
    };
    const Name& TouchFirstMoveEvent::eventName()
    {
        static Name name = "touch-first-move";
        return name;
    };
    const Name& TouchPressureChangeEvent::eventName()
    {
        static Name name = "touch-pressure-change";
        return name;
    };
}