#include "OpenGUI/Style2/Parse/TextParse.h"
#include "OpenGUI/Style2/Parse/MathParse.h"
#include "OpenGUI/Style2/Parse/YogaParse.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Style2/ParseUtils.hpp"
#include "OpenGUI/Style2/Shadow.h"
#include "OpenGUI/Text/TextTypes.h"
#include "YGValue.h"
#include <string_view>
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


bool OGUI::ParseValue(std::string_view str, TextStyle& value)
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

bool OGUI::ParseTextWeight(std::string_view str, int& value)
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


bool OGUI::ParseLineHeight(std::string_view str, float& value)
{
    switchstr(str) //presets
	{
		casestr("normal") value = 1.f; return true;
		default:
			break;
    }
    return OGUI::ParseValue(str, value);
}

namespace std
{
	inline void split_escape(const string_view& s, vector<string_view>& tokens, const string_view& delimiters = " ", const char escape[2] = "()")
	{
		string::size_type escapeStart = s.find_first_of(escape[0], 0);
		string::size_type escapeEnd = s.find_first_of(escape[1], escapeStart);
		string::size_type lastPos = s.find_first_not_of(delimiters, 0);
		string::size_type pos = s.find_first_of(delimiters, lastPos);
		while(pos > escapeStart && pos < escapeEnd)
			pos = s.find_first_of(delimiters, pos+1);
		while (string::npos != pos || string::npos != lastPos)
		{
			auto substr = s.substr(lastPos, pos - lastPos);
			tokens.push_back(substr);//use emplace_back after C++11
			
			string::size_type escapeStart = s.find_first_of(escape[0], pos);
			string::size_type escapeEnd = s.find_first_of(escape[1], escapeStart);
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
			while(pos > escapeStart && pos < escapeEnd)
				pos = s.find_first_of(delimiters, pos+1);
		}
	}
}

bool OGUI::ParseValue(std::string_view str, std::vector<TextShadow>& value)
{
	using namespace std;
	std::vector<std::string_view> array;
	std::split_escape(str, array, ",");
	std::vector<std::string_view> parts;
	for(auto elem : array)
	{
		parts.clear();
		std::split(elem, parts);
		if(parts.size() != 3)
			return false;
		TextShadow e;
		if(!ParseLength(parts[0], e.offset.x))
			return false;
		if(!ParseLength(parts[1], e.offset.y))
			return false;
		if(!ParseValue(parts[2], e.color))
			return false;
		value.push_back(e);
	}
	return true;
}
