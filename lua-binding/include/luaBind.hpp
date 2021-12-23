#pragma once
#include "OpenGUI/Core/olog.h"
#include "sol/sol.hpp"
#include <any>
#include <functional>
#include <typeindex>
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Bind/Bind.h"
namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::Name>
    {
        static int push(lua_State* L, const OGUI::Name& name) {
            auto sv = name.ToStringView();
            std::string utf8;
            sv.encode_to_utf8(utf8);
            int amount = sol::stack::push(L, utf8);
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

using AnyPusherMap = std::unordered_map<std::type_index, int(*)(lua_State* L, const OGUI::any& any)>;
extern AnyPusherMap any_pusher;

namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::any>
    {
        static int push(lua_State* L, const OGUI::any& any) {
            auto iter = any_pusher.find(any.type());
            if(iter == any_pusher.end())
            {
                using namespace ostr::literal;
                OGUI::olog::Warn(u"type {} is not registered to any_pusher!"_o.format(any.type().name()));
                sol::stack::push(L, sol::nil);
                return 1;
            }
            return (iter->second)(L, any);
        }
    };
}

namespace OGUI
{
    template<class T>
    void add_any_pusher()
    {
        any_pusher.emplace(typeid(T), +[](lua_State* L, const OGUI::any& any)
        {
            return sol::stack::push(L, OGUI::any_cast<T>(any));
        });
    }

    void BindLua(lua_State* L);

    struct SubLuaBindable
    {
        struct LuaBindable* owner;
        sol::table table;
        std::string path;
	    sol::object index(sol::string_view key);
	    void new_index(sol::string_view key, sol::object value);
    };

    //TODO: nested binding path, eg. a.b.c
    //TODO: dynamic data source
    struct LuaBindable : Bindable
    {
	    sol::table table;
        sol::table eventHandler;
	    sol::object index(sol::string_view key);
	    void new_index(sol::string_view key, sol::object value);
        bool HandleEvent(Name eventName, IEventArg &args) override;
	    LuaBindable(sol::table inTable, sol::table inHandler);
        virtual ~LuaBindable();
    };
}