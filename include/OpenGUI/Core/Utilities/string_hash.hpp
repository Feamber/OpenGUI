#pragma once
#include "OpenGUI/Core/ostring/helpers.h"

// used for switch stl string
#define switchstr(s) switch(ostr::string str = s; str.get_hash())

// used for switch Name
#define switchname(n) switch(ostr::string_view str = n.ToStringView(); n.GetStringHash())

#define casestr(s) case u ## s ##_o.get_hash(): if(str != u ## s ##_o) break;