%for function in functions:
%for desc in function.descs:
%if desc.comment:
---${desc.comment}
%endif
%for field in desc.fields:
---@param ${field.name} ${field.luaType} ${("@" + field.comment) if field.comment else ""}
%endfor
function ${function.short_name}(${str.join(", ",  [x.name or ("anonymous%d"%i) for i, x in enumerate(desc.fields)])}) end
%endfor
%endfor
