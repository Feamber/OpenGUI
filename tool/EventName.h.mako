//DO NOT MODIFY THIS FILE
//generated from EventName.h.mako
#include "OpenGUI/Core/OName.h"
%for header in db.headers:
#include "${header}"
%endfor

namespace OGUI
{
%for record in db.events:
    OGUI_API const Name& GetEventName(type_t<${record.name}>);
%endfor
}