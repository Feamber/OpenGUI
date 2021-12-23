//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.cpp.mako

#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

%for record in db.records:
bool OGUI::TryGet(const ${record.name}& event, std::string_view name, OGUI::any& out)
{
%if record.isEvent:
    static Name eventName = "${record.event_name}";
    if(name == "eventName")
    {
        out = eventName;
        return true;
    }
%endif
%for base in record.bases:
    if(TryGet((const ${base}&)event, name, out))
        return true;
%endfor
%if record.fields:
    switchstr(name)
    {
    %for field in record.fields:
        casestr("${field.name}") out = event.${field.name}; break;
    %endfor
        default:
            return false;
    }
    return true;
%else:
    return false;
%endif
}
%endfor