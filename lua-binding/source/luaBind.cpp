#include "OpenGUI/Context.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/value.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Style2/ParseUtils.hpp"
#include "OpenGUI/VisualElement.h"
#include "sol/sol.hpp"
#include "luaBind.hpp"
#include <string>
#include <typeindex>
#include "OpenGUI/Event/EventRouter.h"

namespace OGUI
{

	struct LuaEvent : EventBase
	{
		sol::table table;
		LuaEvent(sol::table table)
			:table(table) {}
	};
    const OGUI::Name& GetEventName(type_t<LuaEvent>)
    {
        static OGUI::Name name = "script";
        return name;
    }

	bool TryGet(const LuaEvent& event, ostr::string_view name, OGUI::Meta::Value& out)
	{
		out = event.table[name];
		if(!out)
		    return false;
        return true;
	}
}

sol::object OGUI::LuaBindable::index(sol::string_view key)
{
    if(key == "__raw")
        return table;
    sol::object obj = table[key];
    if(obj.is<sol::table>())
    {
        auto prop = obj.as<sol::table>();
        sol::optional<sol::function> getter = prop["get"];
        if(getter)
            return getter->call(eventHandler);
        return sol::nil;
    }
    return obj;
}

void OGUI::LuaBindable::new_index(sol::string_view key, sol::object value)
{
    sol::object obj = table[key];
    if(obj.is<sol::table>())
    {
        auto prop = obj.as<sol::table>();
        sol::optional<sol::function> setter = prop["set"];
        if(setter)
        {
            setter->call(eventHandler, value);
            Notify(key);
        }
        return;
    }
    table[key] = value;
    Notify(key);
}

bool OGUI::LuaBindable::HandleEvent(Name eventName, IEventArg &args)
{
    bool handled = Bindable::HandleEvent(eventName, args);
    sol::optional<sol::function> function = eventHandler[eventName];
    if(function)
    {
        sol::optional<bool> result;
        result = (*function)(eventHandler, args).get<sol::optional<bool>>();
        if(result)
            handled |= *result;
    }
    return handled;
}
OGUI::LuaBindable::~LuaBindable() {}
OGUI::LuaBindable::LuaBindable(sol::table inTable, sol::table inHandler)
    :table(inTable), eventHandler(inHandler)
{
    for(auto& kv : table)
    {
        auto name = kv.first.as<sol::optional<sol::string_view>>();
        if(!name)
        {
            olog::Warn(u"invalid binding path founded, must be string!"_o);
            continue;
        }
        if(std::starts_with(*name, "__")) //skip
            continue;
        auto value = kv.second;

        if(value.is<sol::function>())
            continue;
        if(value.is<sol::table>())
        {
            sol::table property = value.as<sol::table>();
            sol::object getter = property["get"];
            if(getter.is<sol::function>())
            {
                AddSource({*name, [this, path = *name](const AttrSync& sync)
                {
                    sol::optional<sol::table> prop = table[path];
                    if(!prop)
                    {
                        olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
                        return;
                    }
                    sol::optional<sol::function> getter = (*prop)["get"];
                    if(!getter)
                    {
                        olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
                        return;
                    }
                    auto obj = getter->call(eventHandler).get<Meta::Value>();
                    if(!obj)
                    {
                        olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
                        return;
                    }
                    sync(obj);
                }});
            }
            else 
                olog::Warn(u"invalid property getter founded, must be function!"_o);

            
            sol::object setter = property["set"];
            if(setter.is<sol::function>())
            {
                AddBind({*name, [this, path = *name](Meta::ValueRef inRef)
                {
                    sol::optional<sol::table> prop = table[path];
                    if(!prop)
                    {
                        olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
                        return;
                    }
                    sol::optional<sol::function> setter = (*prop)["set"];
                    if(!setter)
                    {
                        olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
                        return;
                    }
                    setter->call(eventHandler, inRef);
                }});
            }
            else 
                olog::Warn(u"invalid property setter founded, must be function!"_o);

            continue;
        }

        AddBind({
            *name, [this, path = *name](Meta::ValueRef inRef)
            {
                table[path] = inRef;
                auto cbPath = "on" + std::string(path) + "Changed";
                cbPath[2] = std::toupper(cbPath[2]); 
                sol::optional<sol::function> onChange = eventHandler[cbPath];
                if(onChange)
                {
                    (*onChange)(eventHandler, this);
                }
            }
        });
        
        AddSource({*name, [this, path = *name](const AttrSync& sync)
        {
            auto obj = table[path].get<Meta::Value>();
            if(obj)
                sync(obj);
            else
                olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
        }});
    }
}

namespace OGUI::Meta::Lua
{

    template<class T>
    size_t PushImpl(const void* dst, lua_State* L)
    {
        return sol::stack::push(L, *(T*)dst);
    }

    void SharedPtrDtor(void* memory)
    {
        memory = sol::detail::align_usertype_unique<std::shared_ptr<void>, true>(memory);
        auto obj = (std::shared_ptr<void>*)memory;
        obj->~shared_ptr();
    }
        
    std::size_t aligned_space_for(gsl::span<std::pair<size_t, size_t>> types, void* alignment) {
        // use temporary storage to prevent strict UB shenanigans
        char* start = static_cast<char*>(alignment);
        std::vector<char> alignment_shim;
        if(start == nullptr)
        {
            size_t maxAlign = 0, maxSize = 0;
            for(auto& type : types)
            {
                maxSize = std::max(maxSize, type.first);
                maxAlign = std::max(maxAlign, type.second);
            }
            alignment_shim.resize(maxAlign + maxSize);
            start = alignment_shim.data();
        }
        for(auto& type : types)
            sol::detail::align_one(type.second, type.first, alignment);
        return static_cast<char*>(alignment) - start;
    }
    void* allocate_lua_obj(lua_State* L, const Type* type)
    {
        /* the assumption is that `lua_newuserdata` -- unless someone
        passes a specific lua_Alloc that gives us bogus, un-aligned pointers
        -- uses malloc, which tends to hand out more or less aligned pointers to memory
        (most of the time, anyhow)

        but it's not guaranteed, so we have to do a post-adjustment check and increase padding

        we do this preliminarily with compile-time stuff, to see
        if we strike lucky with the allocator and alignment values

        otherwise, we have to re-allocate the userdata and
        over-allocate some space for additional padding because
        compilers are optimized for aligned reads/writes
        (and clang will barf UBsan errors on us for not being aligned)
        */
        std::pair<size_t, size_t> types[] = {{sizeof(void*), alignof(void*)}, {type->Align(), type->Size()}};
        const std::size_t initial_size = aligned_space_for(types, nullptr);
        const std::size_t misaligned_size = aligned_space_for(types, reinterpret_cast<void*>(0x1));

        void* pointer_adjusted;
        void* data_adjusted;
        bool result
                = sol::detail::attempt_alloc(L, alignof(void*), sizeof(void*), type->Align(), type->Size(), initial_size, pointer_adjusted, data_adjusted);
        if (!result) {
            // we're likely to get something that fails to perform the proper allocation a second time,
            // so we use the suggested_new_size bump to help us out here
            pointer_adjusted = nullptr;
            data_adjusted = nullptr;
            result = sol::detail::attempt_alloc(
                    L, alignof(void*), sizeof(void*), type->Align(), type->Size(), misaligned_size, pointer_adjusted, data_adjusted);
            if (!result) {
                if (pointer_adjusted == nullptr) {
                    luaL_error(L, "aligned allocation of userdata block (pointer section) for '%s' failed", type->Name().encode_to_utf8().c_str());
                }
                else {
                    luaL_error(L, "aligned allocation of userdata block (data section) for '%s' failed", type->Name().encode_to_utf8().c_str());
                }
                return nullptr;
            }
        }

        void** pointerpointer = reinterpret_cast<void**>(pointer_adjusted);
        void*& pointerreference = *pointerpointer;
        void* allocationtarget = reinterpret_cast<void*>(data_adjusted);
        pointerreference = allocationtarget;
        return allocationtarget;
    }

	void** allocate_lua_pointer(lua_State* L, const Type* type) {
        static const std::size_t initial_size = sol::detail::aligned_space_for<void*>(nullptr);
        static const std::size_t misaligned_size = sol::detail::aligned_space_for<void*>(reinterpret_cast<void*>(0x1));

        std::size_t allocated_size = initial_size;
        void* unadjusted = lua_newuserdata(L, initial_size);
        void* adjusted = sol::detail::align(alignof(void*), sizeof(void*), unadjusted, allocated_size);
        if (adjusted == nullptr) {
            lua_pop(L, 1);
            // what kind of absolute garbage trash allocator are we dealing with?
            // whatever, add some padding in the case of MAXIMAL alignment waste...
            allocated_size = misaligned_size;
            unadjusted = lua_newuserdata(L, allocated_size);
            adjusted = sol::detail::align(alignof(void*), sizeof(void*), unadjusted, allocated_size);
            if (adjusted == nullptr) {
                // trash allocator can burn in hell
                lua_pop(L, 1);
                // luaL_error(L, "if you are the one that wrote this allocator you should feel bad for doing a
                // worse job than malloc/realloc and should go read some books, yeah?");
                luaL_error(L, "cannot properly align memory for '%s'", type->Name().encode_to_utf8().c_str());
            }
        }
        return static_cast<void**>(adjusted);
    }

    void* usertype_unique_allocate(lua_State* L, void**& pref, sol::detail::unique_destructor*& dx, sol::detail::unique_tag*& id, const Type* type) {
        using namespace sol::detail;
        std::pair<size_t, size_t> types[] = 
        {
            {sizeof(void*), alignof(void*)},
            {sizeof(unique_destructor), alignof(unique_destructor)},
            {sizeof(unique_tag), alignof(unique_tag)},
            {type->Align(), type->Size()},
        };
        const std::size_t initial_size = aligned_space_for(types, nullptr);
        const std::size_t misaligned_size = aligned_space_for(types, reinterpret_cast<void*>(0x1));

        void* pointer_adjusted;
        void* dx_adjusted;
        void* id_adjusted;
        void* data_adjusted;
        bool result = attempt_alloc_unique(L,
                alignof(void*), sizeof(void*), 
                type->Align(), type->Size(),
                initial_size,
                pointer_adjusted,
                dx_adjusted,
                id_adjusted,
                data_adjusted);
        if (!result) {
            // we're likely to get something that fails to perform the proper allocation a second time,
            // so we use the suggested_new_size bump to help us out here
            pointer_adjusted = nullptr;
            dx_adjusted = nullptr;
            id_adjusted = nullptr;
            data_adjusted = nullptr;
            result = attempt_alloc_unique(L,
                    alignof(void*), sizeof(void*), 
                    type->Align(), type->Size(),
                    misaligned_size,
                    pointer_adjusted,
                    dx_adjusted,
                    id_adjusted,
                    data_adjusted);
            if (!result) {
                if (pointer_adjusted == nullptr) {
                    luaL_error(L, "aligned allocation of userdata block (pointer section) for '%s' failed", type->Name().encode_to_utf8().c_str());
                }
                else if (dx_adjusted == nullptr) {
                    luaL_error(L, "aligned allocation of userdata block (deleter section) for '%s' failed", type->Name().encode_to_utf8().c_str());
                }
                else {
                    luaL_error(L, "aligned allocation of userdata block (data section) for '%s' failed", type->Name().encode_to_utf8().c_str());
                }
                return nullptr;
            }
        }

        pref = static_cast<void**>(pointer_adjusted);
        dx = static_cast<unique_destructor*>(dx_adjusted);
        id = static_cast<unique_tag*>(id_adjusted);
        void* mem = static_cast<void*>(data_adjusted);
        return mem;
    }

    int push_ref(lua_State* L, const OGUI::Meta::Type* type, void* dst, bool move)
    {
        using namespace OGUI::Meta;
        using namespace EType;
        using namespace ostr::literal;
        using namespace sol;
        if(type == nullptr)
            return 0;
        switch(type->type)
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
                OGUI::olog::Warn(u"type erased vector is not supported in lua!"_o);
                return 0;
            case _av:
                OGUI::olog::Warn(u"span is not supported in lua!"_o);
                return 0;
            case _e:
            {
                switch(((EnumType*)type)->underlyingType->type)
                {
                    case _i32: 
                        return PushImpl<int32_t>(dst, L);
                    case _i64: 
                        return PushImpl<int64_t>(dst, L);
                    case _u32: 
                        return PushImpl<uint32_t>(dst, L);
                    case _u64: 
                        return PushImpl<uint64_t>(dst, L);
                    default:
                        break;
                }
                return 0;
            }
            case _o:                                                                                         
            {
                if(type == TypeOf<OGUI::Vector2f>::Get())
                {
                    auto& v = *(OGUI::Vector2f*)dst;
                    sol::stack::push(L, v.x);
                    sol::stack::push(L, v.y);
                    return 2;
                }
                auto& obj = (const RecordType&)(*type);
                if(move && !obj.nativeMethods.move)
                {
                    OGUI::olog::Error(u"type \"{}\" is not move constructible!"_o.format(type->Name()));
                    return 0;
                }
                if(!move && !obj.nativeMethods.copy)
                {
                    OGUI::olog::Error(u"type \"{}\" is not copy constructible!"_o.format(type->Name()));
                    return 0;
                }
                using namespace sol;
                auto key = std::string("sol.").append(type->Name().encode_to_utf8());
                if(!luaL_getmetatable(L, key.c_str()))
                {
                    lua_pop(L, 1);
                    OGUI::olog::Error(u"type \"{}\" is not registered to lua!"_o.format(type->Name()));
                    return 0;
                }
                lua_pop(L, 1);
                void* luaObj = Lua::allocate_lua_obj(L, type);
                luaL_getmetatable(L, key.c_str());
                lua_setmetatable(L, -2);
                if(move)
                    obj.nativeMethods.move(luaObj, dst);
                else
                    obj.nativeMethods.copy(luaObj, dst);
                return 1;
            }
            case _r:
            {
                auto& ptr = (const ReferenceType&)(*type);
                if(ptr.pointee->type != _o)
                {
                    if(ptr.ownership == ReferenceType::Shared)
                    {
                        auto& obj = *(std::shared_ptr<void>*)dst;
                        if(!obj)
                            return 0;
                        return push_ref(L, ptr.pointee, obj.get(), move);
                    }
                    else 
                    {
                        auto& obj = *(void**)dst;
                        if(!obj)
                            return 0;
                        return push_ref(L, ptr.pointee, obj, move);
                    }
                }
                if(ptr.ownership == ReferenceType::Shared)
                {
                    auto key = std::string("sol.sol::detail::unique_usertype<").append(ptr.pointee->Name().encode_to_utf8()).append(">");
                    if(!luaL_getmetatable(L, key.c_str()))
                    {
                        lua_pop(L, 1);
                        OGUI::olog::Error(u"type \"{}\" is not registered to lua!"_o.format(ptr.pointee->Name()));
                        return 0;
                    }
                    lua_pop(L, 1);
                    void** pref = nullptr;
                    detail::unique_destructor* fx = nullptr;
                    detail::unique_tag* id = nullptr;
                    void* mem = Lua::usertype_unique_allocate(L, pref, fx, id, type);
                    luaL_getmetatable(L, key.c_str());
                    lua_setmetatable(L, -2);
                    *fx = Lua::SharedPtrDtor;
                    *id = +[](void* source_data, void* target_data, const string_view& ti, const string_view& rebind_ti)
                    {
                        if(rebind_ti != "std::shared_ptr<void>")
                            return 0;
                        return 1;
                    };
                    auto& obj = *(std::shared_ptr<void>*)dst;
                    *pref = obj.get();
                    if(move)
                        new(mem) std::shared_ptr<void>(std::move(obj));
                    else
                        new(mem) std::shared_ptr<void>(obj);
                }
                else 
                {
                    auto key = std::string("sol.").append(ptr.pointee->Name().encode_to_utf8()).append("*");
                    if(!luaL_getmetatable(L, key.c_str()))
                    {
                        lua_pop(L, 1);
                        OGUI::olog::Error(u"type \"{}\" is not registered to lua!"_o.format(ptr.pointee->Name()));
                        return 0;
                    }
                    lua_pop(L, 1);
                    void** ref = Lua::allocate_lua_pointer(L, type);
                    luaL_getmetatable(L, key.c_str());
                    *ref = *(void**)dst;
                    lua_setmetatable(L, -2);
                }
                return 1;
            }
        }
        return 0;
    }
}

void BindLua_generated(lua_State* state);
void OGUI::BindLua(lua_State* state)
{
    sol::state_view lua(state);
    sol::table OGUI = lua.create_named_table("OGUI");
    BindLua_generated(state);
    {
        sol::usertype<VisualElement> type = OGUI["VisualElement"];
        type["GetChildren"] = +[](VisualElement* self)
        {
            std::vector<VisualElement *> children;
            self->GetChildren(children);
            return sol::as_table(children);
        };
    }
    auto type = OGUI.new_usertype<LuaBindable>("LuaBindable", sol::base_classes, sol::bases<Bindable>());
    type[sol::meta_function::index] = &LuaBindable::index;
    type[sol::meta_function::new_index] = &LuaBindable::new_index;
    OGUI["MakeDataModel"] = +[](sol::table table, sol::table eventHandler)
    {
        return std::make_unique<LuaBindable>(table, eventHandler);
    };
	OGUI["ReleaseDataModel"] = +[](std::unique_ptr<Bindable>& dm)
	{
		dm.reset();
	};
    OGUI["RouteEvent"] = +[](VisualElement* target, sol::table table)
    {
        LuaEvent event{table};
        RouteEvent(target, event);
    };
    OGUI["SendEventTo"] = +[](Bindable& target, std::string_view eventName, sol::table table)
    {
        LuaEvent event{table};
        SendEventTo(target, eventName, event);
    };
}