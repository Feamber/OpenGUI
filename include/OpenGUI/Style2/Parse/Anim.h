#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include "OpenGUI/Style2/AnimTypes.h"

namespace OGUI
{
	OGUI_API bool ParseTime(std::string_view str, float& value);
	OGUI_API bool ParseValue(std::string_view str, EAnimDirection& value);
	OGUI_API bool ParseValue(std::string_view str, EAnimResumeMode& value);
	OGUI_API bool ParseValue(std::string_view str, EAnimYieldMode& value);
	OGUI_API bool ParseValue(std::string_view str, EAnimFillMode& value);
	OGUI_API bool ParseValue(std::string_view str, EAnimPlayState& value);
    
	OGUI_API bool ParseValue(std::string_view str, struct AnimTimingFunction& TimingFunction);
	OGUI_API bool ParseIterationCount(std::string_view str, float& value);
}