#pragma once
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{
	using namespace ostr::literal;

	struct olog
	{
		enum class Level : uint8_t
		{
			None,
			Verbose,
			Debug,
			Info,
			Warn,
			Error,
			Fatal,
			MAX
		};

#define __MAKE_LOG_CALL_DECLARE(func)\
static void func(ostr::string_view msg);\
static void func(const ostr::string& msg);\
template<typename...Args>\
static void func(ostr::string_view fmt, Args&&...args)\
{\
	func(fmt.format(std::forward<Args>(args)...));\
}

		__MAKE_LOG_CALL_DECLARE(Verbose)
		__MAKE_LOG_CALL_DECLARE(Debug)
		__MAKE_LOG_CALL_DECLARE(Info)
		__MAKE_LOG_CALL_DECLARE(Warn)
		__MAKE_LOG_CALL_DECLARE(Error)
		__MAKE_LOG_CALL_DECLARE(Fatal)


#undef __MAKE_LOG_CALL_DECLARE

#define OUNREACHABLE { olog::Fatal(u"Unexpected value @ {}"_o, ostr::string(__FUNCTION__)); }
#define OASSERT(x) { if(!x) olog::Fatal(u"Unexpected value @ {}"_o, ostr::string(__FUNCTION__)); }

	};

}