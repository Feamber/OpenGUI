#define DLL_IMPLEMENTATION
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Context.h"

namespace OGUI
{
#define __MAKE_LOG_CALL_IMPL(func)\
    void olog::func(ostr::string_view msg)\
    {\
        Context::Get().logImpl->Log(olog::Level::func, msg);\
    }\
    void olog::func(const ostr::string& msg) \
    {\
        Verbose(msg.to_sv());\
    }

    __MAKE_LOG_CALL_IMPL(Verbose)
    __MAKE_LOG_CALL_IMPL(Debug)
    __MAKE_LOG_CALL_IMPL(Info)
    __MAKE_LOG_CALL_IMPL(Warn)
    __MAKE_LOG_CALL_IMPL(Error)
    __MAKE_LOG_CALL_IMPL(Fatal)

#undef __MAKE_LOG_CALL_IMPL
}
