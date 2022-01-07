//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.cpp.mako

#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/rtti.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

bool OGUI::TryGet(const OGUI::EventBase& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    switchstr(name)
    {
        casestr("currentPhase") out = event.currentPhase; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::FocusDataBase& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("cause") out = event.cause; break;
        casestr("causeDescribe") out = event.causeDescribe; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PreFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::FocusDataBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("currentFocusedPath") out = event.currentFocusedPath; break;
        casestr("newFocusedPath") out = event.newFocusedPath; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PreKeyboardFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::FocusDataBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("currentFocused") out = event.currentFocused; break;
        casestr("newFocused") out = event.newFocused; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::FocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::FocusDataBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("oldFocusedPath") out = event.oldFocusedPath; break;
        casestr("currentFocusedPath") out = event.currentFocusedPath; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::KeyboardFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::FocusDataBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("oldFocused") out = event.oldFocused; break;
        casestr("currentFocused") out = event.currentFocused; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PreGotKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-got-keyboard-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PreKeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreLostKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-lost-keyboard-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PreKeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreGotFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-got-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PreFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreLostFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-lost-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PreFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::GotKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"got-keyboard-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::KeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::LostKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"lost-keyboard-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::KeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::GotFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"got-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::FocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::LostFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"lost-focus";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::FocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreDetachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-detach";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("prevParent") out = event.prevParent; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PreAttachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pre-attach";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("prevParent") out = event.prevParent; break;
        casestr("nextParent") out = event.nextParent; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PostDetachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"post-detach";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("prevParent") out = event.prevParent; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PostAttachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"post-attach";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("prevParent") out = event.prevParent; break;
        casestr("nextParent") out = event.nextParent; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PointerData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("pointerId") out = event.pointerId; break;
        casestr("pointerType") out = event.pointerType; break;
        casestr("isPrimary") out = event.isPrimary; break;
        casestr("isTouch") out = event.isTouch; break;
        casestr("isAbsolute") out = event.isAbsolute; break;
        casestr("isDoubleClick") out = event.isDoubleClick; break;
        casestr("gestureType") out = event.gestureType; break;
        casestr("button") out = event.button; break;
        casestr("position") out = event.position; break;
        casestr("deltaPosition") out = event.deltaPosition; break;
        casestr("wheelOrGestureDelta") out = event.wheelOrGestureDelta; break;
        casestr("accTime") out = event.accTime; break;
        casestr("clickCount") out = event.clickCount; break;
        casestr("pressure") out = event.pressure; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PointerDownEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-down";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerMoveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-move";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerUpEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-up";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerClickEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-click";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerDoubleClickEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-double-click";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerEnterEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-enter";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerLeaveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-leave";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerScrollEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"pointer-scroll";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchGestureEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"touch-gesture";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchFirstMoveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"touch-first-move";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchPressureChangeEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"touch-pressure-change";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyData& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    if(TryGet((const OGUI::EventBase&)event, name, out))
        return true;
    switchstr(name)
    {
        casestr("gestureType") out = event.gestureType; break;
        casestr("key") out = event.key; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::KeyDownEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"key-down";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyUpEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"key-up";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyHoldEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
    static ostr::string_view eventName = u"key-hold";
    if(name == ostr::string_view(u"eventName"))
    {
        out = eventName;
        return true;
    }
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
