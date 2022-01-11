//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
%for header in db.headers:
#include "${header}"
%endfor
#include "luaBind.hpp"
void BindLua_generated(lua_State* L)
{
    sol::state_view lua(L);
    sol::table OGUI = lua["OGUI"];
%for record in db.records:
    {
    %if record.bases:
        sol::usertype<${record.name}> type = 
            OGUI.new_usertype<${record.name}>("${record.short_name}", sol::base_classes, sol::bases<${str.join(", ", [base.name for base in record.bases])}>());
    %else:
        sol::usertype<${record.name}> type = OGUI.new_usertype<${record.name}>("${record.short_name}");
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
            ${desc.getReference(record, method)}
        %endfor
        );
        %else:
        type["${method.short_name}"] = ${method.descs[0].getReference(record, method)};
        %endif
    %endfor
    }
%endfor
%for function in db.functions.values():
    %if len(function.descs) > 1:
    OGUI["${function.short_name}"] = sol::overload(
    %for desc in function.descs:
        ${desc.getReference(None, function)}
    %endfor
    );
    %else:
    OGUI["${function.short_name}"] = ${function.descs[0].getReference(None, function)};
    %endif
%endfor
%for enum in db.enums:
    OGUI.new_enum<int>("${enum.short_name}", {
%for enumerator in enum.enumerators[:-1]:
        {"${enumerator.short_name}", ${enumerator.value}},
%endfor
        {"${enum.enumerators[-1].short_name}", ${enum.enumerators[-1].value}}
    });
%endfor
}
