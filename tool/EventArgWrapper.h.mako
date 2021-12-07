//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
%for header in db.headers:
#include "${header}"
%endfor
#include "OpenGUI/Bind/any.h"

namespace OGUI
{
%for record in db.records:
    OGUI_API bool TryGet(const ${record.name}& event, std::string_view name, OGUI::any& out);
%endfor
}