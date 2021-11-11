#pragma once
#include <string_view>
#include <vector>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Transform.h"

namespace OGUI
{
	bool ParseValue(std::string_view str, float& value);
	bool ParseValue(std::string_view str, int& value);
	bool ParseColorName(std::string_view str, Color4f& value);
	bool ParseTranslate(std::string_view str, Vector2f& t);
	bool ParseValue(std::string_view str, Vector2f& t);
	bool ParseValue(std::string_view str, Color4f& value);
	bool ParseValue(std::string_view str, ComputedTransform& value);
	bool ParseValue(std::string_view str, std::vector<TransformFunction>& value);
}