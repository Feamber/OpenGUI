//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "sol/sol.hpp"
%for header in db.headers:
#include "${header}"
%endfor

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
%for record in db.records:
    {
    %if record.bases:
        sol::usertype<${record.name}> type = 
            lua.new_usertype<${record.name}>("${record.short_name}", sol::base_classes, sol::bases<${str.join(", ", [base.name for base in record.bases])}>());
    %else:
        sol::usertype<${record.name}> type = lua.new_usertype<${record.name}>("${record.short_name}");
    %endif
    %for field in record.fields:
    %if field.getter or field.setter:
        type["${field.name}"] = sol::property(${field.getter or "{}"}, ${field.setter or "{}"});
    %else:
        type["${field.name}"] = &${record.name}::${field.name};
    %endif
    %endfor
    %for method in record.methods:
        %if len(method.descs) > 1:
        type["${method.short_name}"] = sol::overload(
        %for desc in method.descs:
            (${desc.signature})&${method.name}
        %endfor
        );
        %elif method.overloaded:
        type["${method.short_name}"] = (${method.descs[0].signature})&${method.name};
        %else:
        type["${method.short_name}"] = &${method.name};
        %endif
    %endfor
    }
%endfor
%for function in db.functions:
    %if len(function.descs) > 1:
    lua["${function.short_name}"] = sol::overload(
    %for desc in function.descs:
        (%{desc.signature})&${function.name}
    %endfor
    );
    %elif function.overloaded:
    lua["${function.short_name}"] = (${function.descs[0].signature})&${function.name};
    %else:
    lua["${function.short_name}"] = &${function.name};
    %endif
%endfor
%for enum in db.enums:
    {
        auto object = lua.create_table("${enum.short_name}");
    %for enumerator in enum.enumerators:
        object["${enumerator.short_name}"] = ${enumerator.value};
    %endfor
    }
%endfor
}
