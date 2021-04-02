#pragma once
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Configure.h"
#include <cassert>

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
OGUI_API static void func(ostr::string_view msg);\
OGUI_API static void func(const ostr::string& msg);\
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

#define OUNREACHABLE { using namespace ostr::literal; OGUI::olog::Fatal(u"Unexpected value @ {}"_o, ostr::string(__FUNCTION__)); assert(false); }
#define OASSERT(x) { using namespace ostr::literal; if(!(x)){ OGUI::olog::Fatal(u"Unexpected value @ {}"_o, ostr::string(__FUNCTION__)); assert(false); }}

	};

}