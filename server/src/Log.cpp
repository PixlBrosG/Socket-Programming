#include "SmartTV/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Logger {

	static std::vector<spdlog::sink_ptr> s_Sinks;

	std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name)
	{
		auto logger = std::make_shared<spdlog::logger>(name, s_Sinks.begin(), s_Sinks.end());
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::warn);
		spdlog::register_logger(logger);

		return logger;
	}

	std::shared_ptr<spdlog::logger> GetOrCreateLogger(const std::string& name)
	{
		if (auto logger = spdlog::get(name); logger)
			return logger;

		return CreateLogger(name);
	}

	void Init()
	{
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("%^[%T] [%n/%l]: %v%$");

		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("SmartTV.log");
		fileSink->set_pattern("%^[%T] [%n/%l]: %v");

		s_Sinks.emplace_back(consoleSink);
		s_Sinks.emplace_back(fileSink);

		CreateLogger("Server");
	}

}