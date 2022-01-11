---@class OGUI
%for record in db.records:
---@field public ${record.short_name} ${record.luaName} ${("@" + record.comment) if record.comment else ""}
%endfor
%for enum in db.enums:
---@field public ${enum.short_name} ${enum.luaName} ${("@" + enum.comment) if enum.comment else ""}
%endfor
local _OGUI = {}

%for function in db.functions.values():
%for desc in function.descs:
%if desc.comment:
---${desc.comment}
%endif
%for field in desc.fields:
---@param ${field.name} ${field.luaType} ${("@" + field.comment) if field.comment else ""}
%endfor
function _OGUI:${function.short_name}(${str.join(", ",  [x.name or ("anonymous%d"%i) for i, x in enumerate(desc.fields)])}) end
%endfor
%endfor

---@type OGUI
OGUI = {}