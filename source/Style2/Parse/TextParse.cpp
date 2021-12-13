#include "OpenGUI/Style2/Parse/TextParse.h"
#include "OpenGUI/Style2/Parse/MathParse.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Text/TextTypes.h"
#include "YGValue.h"
using namespace ostr::literal;



bool OGUI::ParseValue(std::string_view str, TextAlign& value)
{
    switchstr(str)
	{
		casestr("start") value = TextAlign::Start; return true;
		casestr("end") value = TextAlign::End; return true;
		casestr("left") value = TextAlign::Left; return true;
		casestr("right") value = TextAlign::Right; return true;
		casestr("center") value = TextAlign::Center; return true;
		casestr("justify") value = TextAlign::Justify; return true;
		default:
			break;
	}
	return false;
}


OGUI_API bool OGUI::ParseValue(std::string_view str, TextStyle& value)
{
    switchstr(str)
	{
		casestr("normal") value = TextStyle::Normal; return true;
		casestr("italic") value = TextStyle::Italic; return true;
		default:
			break;
	}
	return false;
}

OGUI_API bool OGUI::ParseTextWeight(std::string_view str, int& value)
{
    switchstr(str) //presets
	{
		casestr("normal") value = 400; return true;
		casestr("bold") value = 700; return true;
		default:
			break;
	}
    return OGUI::ParseValue(str, value);
}


OGUI_API bool OGUI::ParseLineHeight(std::string_view str, float& value)
{
    switchstr(str) //presets
	{
		casestr("normal") value = 1.f; return true;
		default:
			break;
    }
    return OGUI::ParseValue(str, value);
}