%if enum.comment:
---${enum.comment}
%endif
---@class ${enum.luaName}
%for enumerator in enum.enumerators:
---@field public ${enumerator.short_name} integer ${("@" + enumerator.comment) if enumerator.comment else ""}
%endfor
local E${enum.short_name} = {}

---@type ${enum.luaName}
local ${enum.short_name}

return E${enum.short_name}