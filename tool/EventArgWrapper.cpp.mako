//DO NOT MODIFY THIS FILE
//generated from EventArgWrapper.cpp.mako

#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/rtti.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

%for record in db.records:
bool OGUI::TryGet(const ${record.name}& event, ostr::string_view name, OGUI::Meta::ValueRef& out)
{
%if record.isEvent:
    static ostr::string_view eventName = u"${record.event_name}";
    if(name == ostr::string_view(u"eventName"))
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