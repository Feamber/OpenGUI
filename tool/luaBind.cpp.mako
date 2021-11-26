//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "sol/sol.hpp"
%for header in db.headers:
#include "${header}"
%endfor

void Bind(lua_State* L)
{
    sol::state_view lua(L);
%for record in db.records:
    {
        sol::usertype<${record.name}> type = lua.new_usertype<${record.name}>("${record.short_name}");
    %for field in record.fields:
    %if field.getter or field.setter:
        type["${field.name}"] = sol::property(${field.getter or "{}"}, ${field.setter or "{}"});
    %else:
        type["${field.name}"] = &${record.name}::${field.name};
    %endif
    %endfor
    %for method in record.methods:
        type["${method.short_name}"] = &${method.name};
    %endfor
    }
%endfor
}
