
#include "OpenGUI/Style2/Parse/AnimParse.h"
#include "OpenGUI/Style2/Parse/MathParse.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
using namespace ostr::literal;

bool OGUI::ParseTime(std::string_view str, float& value)
{
    std::string_view valuestr;
    float scale = 1;
    if (std::ends_with(str, "s"))
    {
        scale = 1;
        valuestr = str.substr(0, str.length() - 1);
    }
    else if (std::ends_with(str, "ms"))
    {
        scale = 1000;
        valuestr = str.substr(0, str.length() - 2);
    }
    else
        return false;
    if (!ParseValue(valuestr, value))
        return false;
    value /= scale;
    return true;
}
bool OGUI::ParseValue(std::string_view str, EAnimDirection& value)
{
    switchstr(str)
	{
		casestr("normal") value = EAnimDirection::Normal; return true;
		casestr("reverse") value = EAnimDirection::Reverse; return true;
		casestr("alternate") value = EAnimDirection::Alternate; return true;
		casestr("alternate-reverse") value = EAnimDirection::AlternateReverse; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, EAnimResumeMode& value)
{
    switchstr(str)
	{
		casestr("reset") value = EAnimResumeMode::Reset; return true;
		casestr("resume") value = EAnimResumeMode::Resume; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, EAnimYieldMode& value)
{
     switchstr(str)
	{
		casestr("stop") value = EAnimYieldMode::Stop; return true;
		casestr("goback") value = EAnimYieldMode::GoBack; return true;
		casestr("keep") value = EAnimYieldMode::Keep; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, EAnimFillMode& value)
{
    switchstr(str)
	{
		casestr("forwards") value = EAnimFillMode::Forwards; return true;
		casestr("backwards") value = EAnimFillMode::Backwards; return true;
		casestr("none") value = EAnimFillMode::None; return true;
		casestr("both") value = EAnimFillMode::Both; return true;
		default:
			break;
	}
	return false;
}
bool OGUI::ParseValue(std::string_view str, EAnimPlayState& value)
{
    switchstr(str)
	{
		casestr("running") value = EAnimPlayState::Running; return true;
		casestr("paused") value = EAnimPlayState::Paused; return true;
		default:
			break;
	}
	return false;
}

constexpr OGUI::AnimTimingFunction AnimLinearFuncion{};
bool OGUI::ParseValue(std::string_view str, struct AnimTimingFunction& TimingFunction)
{
    //TODO
    TimingFunction = AnimLinearFuncion;
    return true;
}

bool OGUI::ParseIterationCount(std::string_view str, float& value)
{
    if (str == "infinite")
    {
        value = -1;
        return true;
    }
    return ParseValue(str, value);
}