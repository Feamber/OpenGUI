//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "sol/sol.hpp"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/Bind.h"

namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::Name>
    {
        static int push(lua_State* L, const OGUI::Name& name) {
            int amount = sol::stack::push(L, name.ToStringView());
            return amount;
        }
    };

    template<>
    struct unqualified_getter<OGUI::Name>
    {
		static OGUI::Name get(lua_State* L, int index, record& tracking) {
			tracking.use(1);
			size_t sz;
			const char* luastr = lua_tolstring(L, index, &sz);
			return luastr;
		}
    };
}

namespace sol::stack
{
    template<>
    struct unqualified_pusher<ostr::string>
    {
        static int push(lua_State* L, const ostr::string& str) {
            std::string utf8;
            str.to_sv().encode_to_utf8(utf8);
            int amount = sol::stack::push(L, utf8.c_str());
            return amount;
        }
    };

    template<>
    struct unqualified_getter<ostr::string>
    {
		static ostr::string get(lua_State* L, int index, record& tracking) {
			tracking.use(1);
			size_t sz;
			const char* luastr = lua_tolstring(L, index, &sz);
			ostr::string str;
            str.decode_from_utf8(luastr);
            return str;
		}
    };
}

void BindLua(lua_State* L)
{
    sol::state_view lua(L);
    {
        sol::usertype<OGUI::Bindable> type = lua.new_usertype<OGUI::Bindable>("Bindable");
        type["Bind"] = &OGUI::Bindable::Bind;
        type["Unbind"] = &OGUI::Bindable::Unbind;
        type["AddEventBind"] = &OGUI::Bindable::AddEventBind;
    }
    {
        sol::usertype<OGUI::VisualElement> type = 
            lua.new_usertype<OGUI::VisualElement>("VisualElement", sol::base_classes, sol::bases<OGUI::Bindable>());
        type["Visible"] = &OGUI::VisualElement::Visible;
        type["SetVisibility"] = &OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = &OGUI::VisualElement::IsClippingChildren;
        type["PushChild"] = &OGUI::VisualElement::PushChild;
        type["InsertChild"] = &OGUI::VisualElement::InsertChild;
        type["RemoveChild"] = &OGUI::VisualElement::RemoveChild;
        type["GetRoot"] = &OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = &OGUI::VisualElement::GetLayoutRoot;
        type["SetPseudoClass"] = &OGUI::VisualElement::SetPseudoClass;
    }
    lua["QueryFirst"] = (OGUI::VisualElement *(*)(OGUI::VisualElement *, std::basic_string_view<char>))&OGUI::QueryFirst;
    lua["QueryAll"] = (void(*)(OGUI::VisualElement *, std::basic_string_view<char>, std::vector<OGUI::VisualElement *> &))&OGUI::QueryAll;
    lua["BindTree"] = &OGUI::BindTree;
    {
        auto object = lua.create_table("PseudoStates");
        object["None"] = 0;
        object["Active"] = 1;
        object["Hover"] = 2;
        object["Checked"] = 4;
        object["Disabled"] = 8;
        object["Focus"] = 16;
        object["KeyboardFocus"] = 32;
        object["Scroll"] = 64;
        object["Root"] = 128;
    }
}
