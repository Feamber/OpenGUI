#pragma once
#include <string_view>
#include "OpenGUI/Style2/AnimTypes.h"

namespace OGUI
{
	bool ParseTime(std::string_view str, float& value);
	bool ParseValue(std::string_view str, EAnimDirection& value);
	bool ParseValue(std::string_view str, EAnimResumeMode& value);
	bool ParseValue(std::string_view str, EAnimYieldMode& value);
	bool ParseValue(std::string_view str, EAnimFillMode& value);
	bool ParseValue(std::string_view str, EAnimPlayState& value);
    
	bool ParseValue(std::string_view str, struct AnimTimingFunction& TimingFunction);
	bool ParseIterationCount(std::string_view str, float& value);
}