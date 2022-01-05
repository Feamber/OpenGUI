#pragma once
#include "OpenGUI/Core/olog.h"
#include "lua.h"
#include "sol/sol.hpp"
#include <any>
#include <functional>
#include <typeindex>
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Bind/Bind.h"
#ifndef LUABIND_API
#define LUABIND_API
#endif

namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::Name>
    {
        static int push(lua_State* L, const OGUI::Name& name) {
            auto sv = name.ToStringView();
            int amount = sol::stack::push(L, sv.encode_to_utf8());
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
            int amount = sol::stack::push(L, str.to_sv().encode_to_utf8().c_str());
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
            return ostr::string::decode_from_utf8(luastr);
		}
    };
}

using MetatableMap = std::unordered_map<const OGUI::Meta::Type*, const std::string&>;
extern MetatableMap Metatables;

template<class T>
size_t PushImpl(const void* dst, lua_State* L)
{
    return sol::stack::push(L, *(T*)dst);
}

namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::Meta::Value>
    {
        static int push(lua_State* L, const OGUI::Meta::Value& any) {
            using namespace OGUI::Meta::EType;
            using namespace ostr::literal;
            auto dst = any.Ptr();
            switch(any.type->type)
            {
                case _b : 
                    return PushImpl<bool>(dst, L);
                case _i32: 
                    return PushImpl<int32_t>(dst, L);
                case _i64: 
                    return PushImpl<int64_t>(dst, L);
                case _u32: 
                    return PushImpl<uint32_t>(dst, L);
                case _u64: 
                    return PushImpl<uint64_t>(dst, L);
                case _f32: 
                    return PushImpl<float>(dst, L);
                case _f64: 
                    return PushImpl<double>(dst, L);
                case _s: 
                    return PushImpl<ostr::string>(dst, L);
                case _sv: 
                    return PushImpl<ostr::string_view>(dst, L);
                case _a: 
                    OGUI::olog::Warn(u"static array is not supported in lua!"_o);
                    return 0;
                case _da:
                    OGUI::olog::Warn(u"vector is not supported in lua!"_o);
                    return 0;
                case _av:
                    OGUI::olog::Warn(u"span is not supported in lua!"_o);
                    return 0;
                case _o: case _e:
                {
                    
                }
                case _r:
                {

                }
            }
            return 0;
        }
    };

    template<>
    struct unqualified_getter<OGUI::Meta::Value>
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

namespace OGUI
{
    LUABIND_API void BindLua(lua_State* L);

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