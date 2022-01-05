//DO NOT MODIFY THIS FILE
//generated from rtti.h.mako
#include "OpenGUI/Configure.h"
%for header in db.headers:
#include "${header}"
%endfor
#include "OpenGUI/Core/value.h"

%for record in db.records:
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<${record.name}> { static const Type* Get(); };
%if record.hashable:
    OGUI_API size_t Hash(const ${record.name}& value, size_t base = FNV_offset_basis);
%endif
}
%if record.hashable:
namespace std
{
    template<>
    class hash<${record.name}>
    {
        size_t operator()(const ${record.name}& value)
        {
            return OGUI::Meta::Hash(value);
        }
    };
}
%endif
%endfor
%for enum in db.enums:
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<${enum.name}> { static const Type* Get(); };
}
%endfor