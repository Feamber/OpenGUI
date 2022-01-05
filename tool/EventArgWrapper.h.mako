//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
%for header in db.headers:
#include "${header}"
%endfor
#include "OpenGUI/Core/value.h"

namespace OGUI
{
%for record in db.records:
    OGUI_API bool TryGet(const ${record.name}& event, ostr::string_view name, OGUI::Meta::Value& out);
%endfor
}