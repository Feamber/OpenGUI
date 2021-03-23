#pragma once
#include <string_view>

namespace OGUI
{
	struct SystemInterface
	{
		virtual void log(std::string_view str);
	};
}


