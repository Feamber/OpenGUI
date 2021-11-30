//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.cpp.mako
#define DLL_IMPLEMENTATION
#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

bool OGUI::TryGet(const OGUI::EventBase& event, std::string_view name, std::any& out)
{
    switchstr(name)
    {
        casestr("currentPhase") out = event.currentPhase; break;
        default:
            return false;
    }
    return true;
}
bool OGUI::TryGet(const OGUI::PointerData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::PointerDownEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerMoveEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerUpEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerClickEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerDoubleClickEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerEnterEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerLeaveEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PointerScrollEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchGestureEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchFirstMoveEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::TouchPressureChangeEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PointerData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::FocusDataBase& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::PreFocusData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::PreKeyboardFocusData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::PreGotKeyboardFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PreKeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreLostKeyboardFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PreKeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreGotFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PreFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::PreLostFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::PreFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::FocusData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::KeyboardFocusData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::GotKeyboardFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::KeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::LostKeyboardFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::KeyboardFocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::GotFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::FocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::LostFocusEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::FocusData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyData& event, std::string_view name, std::any& out)
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
bool OGUI::TryGet(const OGUI::KeyDownEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyUpEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
bool OGUI::TryGet(const OGUI::KeyHoldEvent& event, std::string_view name, std::any& out)
{
    if(TryGet((const OGUI::KeyData&)event, name, out))
        return true;
    return false;
}
