//DO NOT MODIFY THIS FILE
//generated from lua/bind.cpp.mako
#include "sol/sol.hpp"
#include "OpenGUI/VisualElement.h"

void Bind(lua_State* L)
{
    sol::state_view lua(L);
    {
        sol::usertype<OGUI::VisualElement> type = lua.new_usertype<OGUI::VisualElement>("VisualElement");
        type["Visible"] = &OGUI::VisualElement::Visible;
        type["SetVisibility"] = &OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = &OGUI::VisualElement::IsClippingChildren;
        type["PushChild"] = &OGUI::VisualElement::PushChild;
        type["InsertChild"] = &OGUI::VisualElement::InsertChild;
        type["RemoveChild"] = &OGUI::VisualElement::RemoveChild;
        type["GetRoot"] = &OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = &OGUI::VisualElement::GetLayoutRoot;
    }
}
