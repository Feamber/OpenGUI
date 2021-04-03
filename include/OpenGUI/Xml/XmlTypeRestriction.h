#pragma once
#include <string>
#include "OpenGUI/Core/Containers/vector.hpp"

namespace OGUI
{
	struct XmlTypeRestriction
	{
	public:
		enum class Type
		{
			Null,
			RegexMatch,
			Bounds,
			Enum,
		};

		Type type = Type::Null;

		// RegexMatch
		std::string regex;

		// Bounds
		std::string min;
		std::string max;

		// Enumeration
		std::vector<std::string> enums;
	};
}