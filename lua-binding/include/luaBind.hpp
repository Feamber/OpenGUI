#pragma once
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Core/value.h"
#include "sol/sol.hpp"
#include <any>
#include <functional>
#include <memory>
#include <typeindex>
#include "OpenGUI/Core/OMath.h"
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
    struct unqualified_pusher<ostr::string_view>
    {
        static int push(lua_State* L, const ostr::string_view& str) {
            int amount = sol::stack::push(L, str.encode_to_utf8().c_str());
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

template<class T>
size_t PushImpl(const void* dst, lua_State* L)
{
    return sol::stack::push(L, *(T*)dst);
}

namespace OGUI::Meta::Lua
{
    LUABIND_API std::string GetMetatable(const OGUI::Meta::Type* type);
    void SharedPtrDtor(void* memory);
    std::size_t aligned_space_for(gsl::span<std::pair<size_t, size_t>> types, void* alignment = nullptr);
    void* allocate_lua_obj(lua_State* L, const Type* type);
	void** allocate_lua_pointer(lua_State* L, const Type* type);
    void* usertype_unique_allocate(lua_State* L, void**& pref, sol::detail::unique_destructor*& dx, sol::detail::unique_tag*& id, const Type* type);
    int push_ref(lua_State* L, const OGUI::Meta::Type* type, void* dst, bool move);
}

namespace sol::stack
{
    template<>
    struct unqualified_pusher<OGUI::Meta::ValueRef>
    {
        static int push(lua_State* L, const OGUI::Meta::ValueRef& any) 
        {
            return OGUI::Meta::Lua::push_ref(L, any.type, (void*)any.ptr, false);
        }
    };
        
    template<>
    struct unqualified_pusher<OGUI::Meta::Value>
    {
        static int push(lua_State* L, OGUI::Meta::Value&& any)
        {
            return OGUI::Meta::Lua::push_ref(L, any.type, any.Ptr(), true);
        }

        static int push(lua_State* L, const OGUI::Meta::Value& any) 
        {
            return OGUI::Meta::Lua::push_ref(L, any.type, (void*)any.Ptr(), false);
        }
    };

    template<>
    struct unqualified_getter<OGUI::Meta::Value>
    {
		static OGUI::Meta::Value get(lua_State* L, int index, record& tracking) {
            using namespace OGUI::Meta;
            using namespace EType;
            using namespace ostr::literal;\
            tracking.use(1);
            Value any;
            switch(type_of(L, index))
            {
                case type::boolean:
                {
                    any.Emplace<bool>(lua_toboolean(L, index));
                }
                case type::number:
                {
                    any.Emplace<float>(lua_tonumber(L, index));
                }
                case type::string:
                {
                    size_t sz;
                    const char* luastr = lua_tolstring(L, index, &sz);
                    any.Emplace<ostr::string>(ostr::string::decode_from_utf8(luastr));
                }
                case type::userdata:
                {
                    void* memory = lua_touserdata(L, index);
                    if (lua_getmetatable(L, index) != 1)
                    {
                        lua_pop(L, 1);
                        return {};
                    }

                    lua_getfield(L, -1, "meta");
                    auto type = (const Type*)lua_touserdata(L, -1);
                    lua_pop(L, 2);
                    
					void* dtor = detail::align_usertype_unique_destructor(memory);
                    if(dtor == Lua::SharedPtrDtor)
                    {
                        auto rt = new ReferenceType(ReferenceType::Shared, false, type);
                        rt->temp = true;
                        memory = detail::align_usertype_unique_tag<true, false>(dtor);
					    memory = detail::align_usertype_unique<std::shared_ptr<void>, true, false>(memory);
                        any.type = rt;
                        auto dst = any.Ptr();
                        type->Copy(dst, memory);
                    }
                    else
                    {
                        auto rt = new ReferenceType(ReferenceType::Observed, false, type);
                        rt->temp = true;
                        void* rawdata = detail::align_usertype_pointer(memory);
                        void** pudata = static_cast<void**>(rawdata);
                        any.type = rt;
                        auto dst = any.Ptr();
                        type->Copy(dst, pudata);
                    }

                    return any;
                }
                default:
                    break;
            }
			return any;
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