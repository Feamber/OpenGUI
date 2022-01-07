//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/AttachEvent.h"

namespace OGUI
{
    const Name& OGUI::PreGotKeyboardFocusEvent::GetEventName() 
    {
        static Name name = "pre-got-keyboard-focus";
        return name;
    };
    const Name& OGUI::PreLostKeyboardFocusEvent::GetEventName() 
    {
        static Name name = "pre-lost-keyboard-focus";
        return name;
    };
    const Name& OGUI::PreGotFocusEvent::GetEventName() 
    {
        static Name name = "pre-got-focus";
        return name;
    };
    const Name& OGUI::PreLostFocusEvent::GetEventName() 
    {
        static Name name = "pre-lost-focus";
        return name;
    };
    const Name& OGUI::GotKeyboardFocusEvent::GetEventName() 
    {
        static Name name = "got-keyboard-focus";
        return name;
    };
    const Name& OGUI::LostKeyboardFocusEvent::GetEventName() 
    {
        static Name name = "lost-keyboard-focus";
        return name;
    };
    const Name& OGUI::GotFocusEvent::GetEventName() 
    {
        static Name name = "got-focus";
        return name;
    };
    const Name& OGUI::LostFocusEvent::GetEventName() 
    {
        static Name name = "lost-focus";
        return name;
    };
    const Name& OGUI::PreDetachEvent::GetEventName() 
    {
        static Name name = "pre-detach";
        return name;
    };
    const Name& OGUI::PreAttachEvent::GetEventName() 
    {
        static Name name = "pre-attach";
        return name;
    };
    const Name& OGUI::PostDetachEvent::GetEventName() 
    {
        static Name name = "post-detach";
        return name;
    };
    const Name& OGUI::PostAttachEvent::GetEventName() 
    {
        static Name name = "post-attach";
        return name;
    };
    const Name& OGUI::PointerDownEvent::GetEventName() 
    {
        static Name name = "pointer-down";
        return name;
    };
    const Name& OGUI::PointerMoveEvent::GetEventName() 
    {
        static Name name = "pointer-move";
        return name;
    };
    const Name& OGUI::PointerUpEvent::GetEventName() 
    {
        static Name name = "pointer-up";
        return name;
    };
    const Name& OGUI::PointerClickEvent::GetEventName() 
    {
        static Name name = "pointer-click";
        return name;
    };
    const Name& OGUI::PointerDoubleClickEvent::GetEventName() 
    {
        static Name name = "pointer-double-click";
        return name;
    };
    const Name& OGUI::PointerEnterEvent::GetEventName() 
    {
        static Name name = "pointer-enter";
        return name;
    };
    const Name& OGUI::PointerLeaveEvent::GetEventName() 
    {
        static Name name = "pointer-leave";
        return name;
    };
    const Name& OGUI::PointerScrollEvent::GetEventName() 
    {
        static Name name = "pointer-scroll";
        return name;
    };
    const Name& OGUI::TouchGestureEvent::GetEventName() 
    {
        static Name name = "touch-gesture";
        return name;
    };
    const Name& OGUI::TouchFirstMoveEvent::GetEventName() 
    {
        static Name name = "touch-first-move";
        return name;
    };
    const Name& OGUI::TouchPressureChangeEvent::GetEventName() 
    {
        static Name name = "touch-pressure-change";
        return name;
    };
    const Name& OGUI::KeyDownEvent::GetEventName() 
    {
        static Name name = "key-down";
        return name;
    };
    const Name& OGUI::KeyUpEvent::GetEventName() 
    {
        static Name name = "key-up";
        return name;
    };
    const Name& OGUI::KeyHoldEvent::GetEventName() 
    {
        static Name name = "key-hold";
        return name;
    };
}