#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/VisualElement.h"
#include "sol/sol.hpp"
#include "luaBind.hpp"
#include <string>
#include <typeindex>
#include "OpenGUI/Event/EventRouter.h"
AnyPusherMap any_pusher;

std::type_index GetCppType(sol::type type, size_t& size)
{
	switch(type)
	{
		case sol::type::boolean:
			size = sizeof(bool);
			return typeid(bool);
		case sol::type::string:
			size = sizeof(std::string);
			return typeid(std::string);
		case sol::type::number:
			size = sizeof(double);
			return typeid(double);
		default:
			return typeid(nullptr_t);
	}
}
namespace OGUI
{

	struct LuaEvent : EventBase
	{
		sol::table table;
		LuaEvent(sol::table table)
			:table(table) {}

		static const OGUI::Name& GetEventName()
		{
			static OGUI::Name name = "script";
			return name;
		}
	};


	bool TryGet(const LuaEvent& event, std::string_view name, OGUI::any& out)
	{
		sol::object value = event.table[name];
		switch(value.get_type())
		{
			case sol::type::boolean:
				out = value.as<bool>();
				return true;
			case sol::type::string:
				out = value.as<std::string>();
				return true;
			case sol::type::number:
				out = value.as<double>();
				return true;
			case sol::type::userdata:
			{
				auto userdata = value.as<sol::userdata>();
				auto L =userdata.lua_state();
				sol::optional<sol::table> type = userdata["__type"];
				if(!type)
					break;
				userdata.push();
				void* pointer = lua_touserdata(L, -1);
				pointer = sol::detail::align_usertype_pointer( pointer ); 
				sol::light<std::type_info> info = (*type)["typeid"];
				out = OGUI::any{*(void**)pointer, info};
				return true;
			}
			default:
				return false;
		}
		return false;
	}
}

sol::object OGUI::LuaBindable::index(sol::string_view key)
{
    return table[key];
}

void OGUI::LuaBindable::new_index(sol::string_view key, sol::object value)
{
    table[key] = value;
    Notify(key);
}

OGUI::LuaBindable::LuaBindable(sol::table inTable)
    :table(std::move(inTable))
{
    for(auto& kv : table)
    {
        auto name = kv.first.as<sol::optional<sol::string_view>>();
        if(!name)
        {
            olog::Warn(u"invalid binding path founded, must be string!"_o);
            continue;
        }
        auto value = kv.second;
        
        if(value.is<sol::function>())
        {
            AddEventBind(*name, [this, path = *name](IEventArg& args)
            {
                sol::optional<bool> result = table[path](this, args);
                return result.value_or(false);
            });
            continue;
        }
        size_t size;
        auto luaType = value.get_type();
        auto type = GetCppType(luaType, size);
        
        if(type == typeid(nullptr_t))
        {
            olog::Warn(u"unsupported binding type, binding path:{}"_o.format(*name));
            continue;
        }
        AddBind({
            *name, type, size, [this, luaType, path = *name](const void* data)
            {
                switch(luaType)
                {
                    case sol::type::boolean:
                    {
                        table[path] = *(const bool*)data;
                        break;
                    }
                    case sol::type::string:
                    {
                        table[path] = *(const std::string*)data;
                        break;
                    }
                    case sol::type::number:
                    {
                        table[path] = *(const double*)data;
                        break;
                    }
                    default:
                        return;
                }
                auto cbPath = "on" + std::string(path) + "Changed";
                cbPath[2] = std::toupper(cbPath[2]); 
                sol::optional<sol::function> onChange = table[cbPath];
                if(onChange)
                    (*onChange)(table);
            }
        });
        
        AddSource({*name, [this, path = *name](const AttrSync& sync)
        {
            sol::object obj = table[path];
            sol::userdata ud;
            if(obj != sol::nil)
            {
                switch(obj.get_type())
                {
                    case sol::type::boolean:
                    {
                        auto value = obj.as<bool>();
                        sync(typeid(bool), &value);
                        return;
                    }
                    case sol::type::string:
                    {
                        auto value = obj.as<std::string>();
                        sync(typeid(std::string), &value);
                        return;
                    }
                    case sol::type::number:
                    {
                        auto value = obj.as<double>();
                        sync(typeid(double), &value);
                        return;
                    }
                    default:
                        break;
                }
            }
            olog::Warn(u"binding path {} is not valid anymore!"_o.format(path));
        }});
    }
}
void BindLua_generated(lua_State* state);
void OGUI::BindLua(lua_State* state)
{
    BindLua_generated(state);
    sol::state_view lua(state);

    auto type = lua.new_usertype<LuaBindable>("LuaBindable");
    type[sol::meta_function::index] = &LuaBindable::index;
    type[sol::meta_function::new_index] = &LuaBindable::new_index;
    lua["MakeDataModel"] = +[](sol::table table)
    {
        std::unique_ptr<Bindable> dm = std::make_unique<LuaBindable>(table);
        return dm;
    };
    lua["RouteEvent"] = +[](VisualElement* target, sol::table table)
    {
        LuaEvent event{table};
        RouteEvent(target, event);
    };
    lua["SendEventTo"] = +[](Bindable& target, std::string_view eventName, sol::table table)
    {
        LuaEvent event{table};
        SendEventTo(target, eventName, event);
    };
}