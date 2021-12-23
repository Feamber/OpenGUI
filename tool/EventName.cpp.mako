//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.h.mako
%for header in db.headers:
#include "${header}"
%endfor

namespace OGUI
{
%for record in db.events:
    const Name& ${record.name}::GetEventName() 
    {
        static Name name = "${record.event_name}";
        return name;
    };
%endfor
}