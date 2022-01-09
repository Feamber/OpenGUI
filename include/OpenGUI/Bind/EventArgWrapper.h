//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/AttachEvent.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/AnimEvent.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Core/value.h"

namespace OGUI
{
    OGUI_API bool TryGet(const OGUI::EventBase& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::FocusDataBase& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreKeyboardFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::FocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::KeyboardFocusData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreGotKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreLostKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreGotFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreLostFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::GotKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::LostKeyboardFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::GotFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::LostFocusEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreDetachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PreAttachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PostDetachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PostAttachEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::KeyData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::KeyDownEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::KeyUpEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::KeyHoldEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerDownEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerMoveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerUpEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerClickEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerDoubleClickEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerEnterEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerLeaveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::PointerScrollEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::TouchGestureEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::TouchFirstMoveEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::TouchPressureChangeEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::AnimEventData& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::AnimStartEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::AnimEndEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
    OGUI_API bool TryGet(const OGUI::AnimStopEvent& event, ostr::string_view name, OGUI::Meta::ValueRef& out);
}