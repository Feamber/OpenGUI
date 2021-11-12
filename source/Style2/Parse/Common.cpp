#define DLL_IMPLEMENTATION
#include "OpenGUI/Style2/Parse/Common.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
using namespace ostr::literal;

bool OGUI::ParseValue(std::string_view str, std::string& value)
{
    value = {str.begin(), str.end()};
    return true;
}
bool OGUI::ParseUrl(std::string_view str, std::string& value)
{
    if (std::starts_with(str, "url(") && std::ends_with(str, ")"))
    {
        auto valuestr = str.substr(4, str.length() - 5);
        value = {valuestr.begin(), valuestr.end()};
        return true;
    }
    return false;
}
bool OGUI::ParseValue(std::string_view str, StyleKeyword& value)
{  
    switchstr(str)
	{
		casestr("inherit") value = StyleKeyword::Inherit; return true;
		casestr("unset") value = StyleKeyword::Unset; return true;
		casestr("initial") value = StyleKeyword::Initial; return true;
		default:
			break;
	}
	return false;
}