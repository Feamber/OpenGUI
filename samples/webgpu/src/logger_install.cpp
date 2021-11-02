#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <Windows.h>
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Context.h"

using namespace OGUI;

struct SpdlogLogger : LogInterface
{
	SpdlogLogger()
	{
		auto console_sink = get_console_sink();
		console_sink->set_level(spdlog::level::info);
		console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

		auto file_sink = get_file_sink();
		file_sink->set_level(spdlog::level::trace);
		file_sink->set_pattern("[%Y-%m-%d %z %X.(%F)] [%^%l%$] [%P.%t] %v");

		auto logger = std::make_shared<spdlog::logger>("logger_default", spdlog::sinks_init_list{ console_sink, file_sink });
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::info);
		spdlog::set_default_logger(logger);

		SetConsoleOutputCP(65001);
	}

	virtual void Log(olog::Level l, ostr::string_view msg)
	{
		std::wstring str(msg.raw().cbegin(), msg.raw().cend());
		spdlog::level::level_enum spdlevel = 
		[l]
		{
			if (l == olog::Level::None) return spdlog::level::off;
			return (spdlog::level::level_enum)((uint8_t)l - 1);
		}();
		spdlog::log(spdlevel, std::wstring_view(str));
	}

	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> get_console_sink()
	{
		static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _static = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		return _static;
	}

	std::shared_ptr<spdlog::sinks::daily_file_sink_mt> get_file_sink()
	{
		static std::shared_ptr<spdlog::sinks::daily_file_sink_mt> _static = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
			"Daily.log",        // file name
			0,                  // hour
			0,                  // minute
			false,              // truncate
			7                   // max files
			);
		return _static;
	}
};

static bool bLoggerInstalled = []()-> bool {
    auto& ctx = OGUI::Context::Get();
    ctx.logImpl = std::make_unique<SpdlogLogger>();
    return ctx.logImpl.get();
}(); 