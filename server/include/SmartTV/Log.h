#pragma once

#include <memory>
#include <string>

#include <spdlog/spdlog.h>

namespace Logger {

	inline std::shared_ptr<spdlog::logger> GetLogger(const std::string& name)
	{
		return spdlog::get(name);
	}

	std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name);
	std::shared_ptr<spdlog::logger> GetOrCreateLogger(const std::string& name);

	inline bool LoggerExists(const std::string& name)
	{
		return spdlog::get(name) != nullptr;
	}

	void Init();

#define DEFINE_LOG_DOMAIN(domain) \
	namespace domain { \
		template<typename... Args> inline void Trace(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->trace(fmt, std::forward<Args>(args)...); \
		} \
		template<typename... Args> inline void Info(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->info(fmt, std::forward<Args>(args)...); \
		} \
		template<typename... Args> inline void Warn(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->warn(fmt, std::forward<Args>(args)...); \
		} \
		template<typename... Args> inline void Error(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->error(fmt, std::forward<Args>(args)...); \
		} \
		template<typename... Args> inline void Critical(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->critical(fmt, std::forward<Args>(args)...); \
		} \
		template<typename... Args> inline void Debug(fmt::format_string<Args...> fmt, Args&&... args) { \
			GetLogger(#domain)->debug(fmt, std::forward<Args>(args)...); \
		} \
	}

	DEFINE_LOG_DOMAIN(Server)

}