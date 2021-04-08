#pragma once
#include "OpenGUI/Core/ostring/helpers.h"

// used for switch stl string
#define switchstr(s) switch(ostr::string __str = s; __str.get_hash())

// used for switch Name
#define switchname(n) switch(ostr::string_view __str = n.ToStringView(); n.GetStringHash())

#define casestr(s) case u ## s ##_o.get_hash(): if(__str != u ## s ##_o) break;