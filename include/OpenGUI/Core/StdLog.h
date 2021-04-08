#pragma once
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Interface/Interfaces.h"

namespace OGUI
{

	struct StdOutputLog : LogInterface
	{
		virtual ~StdOutputLog();

		virtual void Log(olog::Level l, ostr::string_view msg) override;
	};

}
