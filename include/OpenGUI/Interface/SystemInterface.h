#pragma once
#include <string_view>
#include <stdint.h>

namespace OGUI
{
	using FileHandle = void*;
	struct SystemInterface
	{
		virtual ~SystemInterface();
		virtual void log(std::string_view str) = 0;
	};

}


