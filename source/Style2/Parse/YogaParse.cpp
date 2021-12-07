
#include "OpenGUI/Style2/Parse/YogaParse.h"
#include "OpenGUI/Style2/Parse/MathParse.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
using namespace ostr::literal;

bool OGUI::ParseValue(std::string_view str, YGValue& value)
{
    if (str == "auto")
    {
        value = YGValueAuto;
        return true;
    }
    if (str == "none")
    {
        value = YGValueUndefined;
        return true;
    }
    float comp;
    std::string numstr;
    YGUnit unit;
    if (ends_with(str, "%"))
    {
        numstr = str.substr(0, str.length() - 1);
        unit = YGUnitPercent;
    }
    else if (ends_with(str, "px"))
    {
        numstr = str.substr(0, str.length() - 2);
        unit = YGUnitPoint;
    }
    else
        return false;

    if (ParseValue(numstr, comp))
    {
        value = YGValue{comp, unit};
        return true;
    }
    else
        return false;
}
bool OGUI::ParseValue(std::string_view str, YGPositionType& value)
{
    switchstr(str)
	{
		casestr("static") value = YGPositionTypeStatic; return true;
		casestr("relative") value = YGPositionTypeRelative; return true;
		casestr("absolute") value = YGPositionTypeAbsolute; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, StyleOverflow& value)
{
    switchstr(str)
	{
		casestr("visible") value = StyleOverflow::Visible; return true;
		casestr("hidden") value = StyleOverflow::Hidden; return true;
		casestr("scroll") value = StyleOverflow::Scroll; return true;
		casestr("clip") value = StyleOverflow::Clip; return true;
		casestr("auto") value = StyleOverflow::Auto; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, YGAlign& value)
{
    switchstr(str)
	{
		casestr("auto") value = YGAlignAuto; return true;
		casestr("flex-start") value = YGAlignFlexStart; return true;
		casestr("center") value = YGAlignCenter; return true;
		casestr("flex-end") value = YGAlignFlexEnd; return true;
		casestr("stretch") value = YGAlignStretch; return true;
		casestr("baseline") value = YGAlignBaseline; return true;
		casestr("space-between") value = YGAlignSpaceBetween; return true;
		casestr("space-around") value = YGAlignSpaceAround; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, YGFlexDirection& value)
{
    switchstr(str)
	{
		casestr("column") value = YGFlexDirectionColumn; return true;
		casestr("column-reverse") value = YGFlexDirectionColumnReverse; return true;
		casestr("row") value = YGFlexDirectionRow; return true;
		casestr("row-reverse") value = YGFlexDirectionRowReverse; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, YGJustify& value)
{
    switchstr(str)
	{
		casestr("flex-start") value = YGJustifyFlexStart; return true;
		casestr("center") value = YGJustifyCenter; return true;
		casestr("flex-end") value = YGJustifyFlexEnd; return true;
		casestr("space-between") value = YGJustifySpaceBetween; return true;
		casestr("space-around") value = YGJustifySpaceAround; return true;
		casestr("space-evenly") value = YGJustifySpaceEvenly; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, YGWrap& value)
{
    switchstr(str)
	{
		casestr("no-wrap") value = YGWrapNoWrap; return true;
		casestr("wrap") value = YGWrapWrap; return true;
		casestr("wrap-reverse") value = YGWrapWrapReverse; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, YGDisplay& value)
{
    switchstr(str)
	{
		casestr("flex") value = YGDisplayFlex; return true;
		casestr("none") value = YGDisplayNone; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseFourSides(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom)
{
    std::vector<std::string_view> tokens;
    std::split(str, tokens, ",");
    if (tokens.size() == 1)
    {
        if (!ParseValue(tokens[0], left))
            return false;
        top = right = bottom = left;
        return true;
    }
    else if (tokens.size() == 2)
    {
        if (!ParseValue(tokens[0], top))
            return false;
        bottom = top;
        if (!ParseValue(tokens[1], left))
            return false;
        right = left;
        return true;
    }
    else if (tokens.size() == 3)
    {
        if (!ParseValue(tokens[0], top))
            return false;
        if (!ParseValue(tokens[1], left))
            return false;
        right = left;
        if (!ParseValue(tokens[2], bottom))
            return false;
        return true;
    }
    else if (tokens.size() == 4)
    {
        if (!ParseValue(tokens[0], top))
            return false;
        if (!ParseValue(tokens[1], left))
            return false;
        if (!ParseValue(tokens[2], right))
            return false;
        if (!ParseValue(tokens[3], bottom))
            return false;
        return true;
    }
    else
        return false;
}