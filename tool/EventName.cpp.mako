//DO NOT MODIFY THIS FILE
//generated from EventName.cpp.mako
#include "OpenGUI/Event/EventName.h"

namespace OGUI
{
%for record in db.events:
    const Name& GetEventName(type_t<${record.name}>) 
    {
        static Name name = "${record.event_name}";
        return name;
    };
%endfor
}