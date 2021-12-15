//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "sol/sol.hpp"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Bind/EventArg.h"
#include "luaBind.hpp"
void BindLua_generated(lua_State* L)
{
    sol::state_view lua(L);
    {
        sol::usertype<OGUI::IEventArg> type = lua.new_usertype<OGUI::IEventArg>("IEventArg");
        type["TryGet"] = (OGUI::any_detail::any(OGUI::IEventArg::*)(std::basic_string_view<char>))&OGUI::IEventArg::TryGet;
    }
    {
        sol::usertype<OGUI::Bindable> type = lua.new_usertype<OGUI::Bindable>("Bindable");
        type["Bind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    {
        sol::usertype<OGUI::VisualElement> type = 
            lua.new_usertype<OGUI::VisualElement>("VisualElement", sol::base_classes, sol::bases<OGUI::Bindable>());
        type["GetName"] = &OGUI::VisualElement::GetName;
        type["SetName"] = &OGUI::VisualElement::SetName;
        type["Visible"] = (bool(OGUI::VisualElement::*)())&OGUI::VisualElement::Visible;
        type["SetVisibility"] = (void(OGUI::VisualElement::*)(bool))&OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = (bool(OGUI::VisualElement::*)())&OGUI::VisualElement::IsClippingChildren;
        type["PushChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::PushChild;
        type["InsertChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *, int))&OGUI::VisualElement::InsertChild;
        type["RemoveChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::RemoveChild;
        type["GetRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetLayoutRoot;
        type["SetPseudoClass"] = (void(OGUI::VisualElement::*)(OGUI::PseudoStates, bool))&OGUI::VisualElement::SetPseudoClass;
        type["Bind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    lua["QueryFirst"] = (OGUI::VisualElement *(*)(OGUI::VisualElement *, std::basic_string_view<char>))&OGUI::QueryFirst;
    lua["QueryAll"] = (void(*)(OGUI::VisualElement *, std::basic_string_view<char>, std::vector<OGUI::VisualElement *> &))&OGUI::QueryAll;
    lua["BindTree"] = (void(*)(OGUI::VisualElement *, OGUI::Bindable &))&OGUI::BindTree;
    lua.new_enum<int>("EventRoutePhase", {
        {"None", 0},
        {"TrickleDown", 1},
        {"Reach", 2},
        {"Broadcast", 4},
        {"BubbleUp", 8},
        {"All", 15},
        {"NoBroadcast", 11}
    });
    lua.new_enum<int>("PseudoStates", {
        {"None", 0},
        {"Active", 1},
        {"Hover", 2},
        {"Checked", 4},
        {"Disabled", 8},
        {"Focus", 16},
        {"KeyboardFocus", 32},
        {"Scroll", 64},
        {"Root", 128}
    });
    lua.new_enum<int>("FocusChangeCause", {
        {"UserActions", 0},
        {"FocusNavigation", 1},
        {"ActivateWindow", 2}
    });
    OGUI::add_any_pusher<float>();
    OGUI::add_any_pusher<double>();
    OGUI::add_any_pusher<bool>();
    OGUI::add_any_pusher<int>();
    OGUI::add_any_pusher<int32_t>();
    OGUI::add_any_pusher<uint32_t>();
    OGUI::add_any_pusher<size_t>();
    OGUI::add_any_pusher<int64_t>();
    OGUI::add_any_pusher<uint64_t>();
    OGUI::add_any_pusher<char>();
    OGUI::add_any_pusher<OGUI::Name>();
    OGUI::add_any_pusher<std::basic_string_view<char>>();
    OGUI::add_any_pusher<std::basic_string<char>>();
    OGUI::add_any_pusher<ostr::string>();
    OGUI::add_any_pusher<OGUI::EventRoutePhase>();
    OGUI::add_any_pusher<OGUI::EGestureEvent>();
    OGUI::add_any_pusher<OGUI::EMouseKey>();
    OGUI::add_any_pusher<OGUI::Vector<float, 2>>();
    OGUI::add_any_pusher<OGUI::FocusChangeCause>();
    OGUI::add_any_pusher<const std::vector<OGUI::VisualElement *> *>();
    OGUI::add_any_pusher<OGUI::VisualElement *>();
    OGUI::add_any_pusher<OGUI::IEventArg*>();
    OGUI::add_any_pusher<OGUI::Bindable*>();
    OGUI::add_any_pusher<OGUI::VisualElement*>();
    OGUI::add_any_pusher<OGUI::EKeyCode>();
    OGUI::add_any_pusher<OGUI::PseudoStates>();
}
