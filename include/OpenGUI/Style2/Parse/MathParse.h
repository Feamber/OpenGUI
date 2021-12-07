#pragma once
#include "OpenGUI/Configure.h"
#include <string_view>
#include <vector>
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Transform.h"

namespace OGUI
{
	OGUI_API bool ParseValue(std::string_view str, float& value);
	OGUI_API bool ParseValue(std::string_view str, int& value);
	OGUI_API bool ParseColorName(std::string_view str, Color4f& value);
	OGUI_API bool ParseTranslate(std::string_view str, Vector2f& t);
	OGUI_API bool ParseValue(std::string_view str, Vector2f& t);
	OGUI_API bool ParseValue(std::string_view str, Color4f& value);
	OGUI_API bool ParseValue(std::string_view str, std::vector<TransformFunction>& value);
}