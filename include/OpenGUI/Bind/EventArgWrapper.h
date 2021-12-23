//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/AttachEvent.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Bind/any.h"

namespace OGUI
{
    OGUI_API bool TryGet(const OGUI::EventBase& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::FocusDataBase& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreFocusData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreKeyboardFocusData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::FocusData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::KeyboardFocusData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreGotKeyboardFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreLostKeyboardFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreGotFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreLostFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::GotKeyboardFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::LostKeyboardFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::GotFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::LostFocusEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreDetachEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PreAttachEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PostDetachEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PostAttachEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerDownEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerMoveEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerUpEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerClickEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerDoubleClickEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerEnterEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerLeaveEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::PointerScrollEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::TouchGestureEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::TouchFirstMoveEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::TouchPressureChangeEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::KeyData& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::KeyDownEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::KeyUpEvent& event, std::string_view name, OGUI::any& out);
    OGUI_API bool TryGet(const OGUI::KeyHoldEvent& event, std::string_view name, OGUI::any& out);
}