//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "sol/sol.hpp"
%for header in db.headers:
#include "${header}"
%endfor
#include "luaBind.hpp"
void BindLua_generated(lua_State* L)
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
    %for field in record.allFields():
    %if field.getter or field.setter:
        type["${field.name}"] = sol::property(${field.getter or "{}"}, ${field.setter or "{}"});
    %else:
        type["${field.name}"] = &${record.name}::${field.name};
    %endif
    %endfor
    %for method in record.allMethods().values():
        %if len(method.descs) > 1:
        type["${method.short_name}"] = sol::overload(
        %for desc in method.descs:
            (${desc.getSignature(record)})&${method.name}
        %endfor
        );
        %else:
        type["${method.short_name}"] = (${method.descs[0].getSignature(record)})&${method.name};
        %endif
    %endfor
    }
%endfor
%for function in db.functions.values():
    %if len(function.descs) > 1:
    lua["${function.short_name}"] = sol::overload(
    %for desc in function.descs:
        (%{desc.getSignature(None)})&${function.name}
    %endfor
    );
    %else:
    lua["${function.short_name}"] = (${function.descs[0].getSignature(None)})&${function.name};
    %endif
%endfor
%for enum in db.enums:
    lua.new_enum<int>("${enum.short_name}", {
%for enumerator in enum.enumerators[:-1]:
        {"${enumerator.short_name}", ${enumerator.value}},
%endfor
        {"${enum.enumerators[-1].short_name}", ${enum.enumerators[-1].value}}
    });
%endfor
%for type in db.event_arg_types:
    OGUI::add_any_pusher<${type}>();
%endfor
}
