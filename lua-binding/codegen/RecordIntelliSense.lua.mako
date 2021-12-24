

%if record.comment:
---${record.comment}
%endif
---@class ${record.luaName} ${(": " + record.bases[0].luaName) if record.bases else ""}
%for field in record.fields:
---@field public ${field.name} ${field.luaType} ${("@" + field.comment) if field.comment else ""}
%endfor
local ${record.short_name} = {}

%for method in record.methods.values():
%for desc in method.descs:
%if desc.comment:
---${desc.comment}
%endif
%for field in desc.fields:
---@param ${field.name} ${field.luaType} ${("@" + field.comment) if field.comment else ""}
%endfor
function ${record.short_name}:${method.short_name}(${str.join(", ",  [x.name or ("anonymous%d"%i) for i, x in enumerate(desc.fields)])}) end
%endfor
%endfor

return ${record.short_name}