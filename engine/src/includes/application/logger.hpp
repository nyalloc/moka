#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace moka
{
	struct application_traits
	{
#ifdef NDEBUG
		constexpr static bool is_debug_build = false;
#endif

#ifndef NDEBUG
		constexpr static bool is_debug_build = true;
#endif
	};

	enum class log_level
	{
		trace,
		debug,
		info,
		warn,
		error
	};

	constexpr log_level default_log_level()
	{
		return application_traits::is_debug_build ? log_level::debug : log_level::info;
	}

	class logger
	{
		std::shared_ptr<spdlog::logger> logger_;
	public:
		explicit logger(const char* name, log_level level = default_log_level())
			: logger_(spdlog::stdout_color_mt(name))
		{
			switch (level)
			{
			case log_level::debug:
				logger_->set_level(spdlog::level::level_enum::debug);
				break;
			case log_level::error:
				logger_->set_level(spdlog::level::level_enum::err);
				break;
			case log_level::info:
				logger_->set_level(spdlog::level::level_enum::info);
				break;
			case log_level::trace:
				logger_->set_level(spdlog::level::level_enum::trace);
				break;
			case log_level::warn:
				logger_->set_level(spdlog::level::level_enum::warn);
				break;
			}
		}

		template<typename... Args>
		void log(log_level level, const char* fmt, Args&&... args);

		template<typename... Args>
		void debug(const char* fmt, Args&&... args);

		template<typename... Args>
		void trace(const char* fmt, Args&&... args);

		template<typename... Args>
		void info(const char* fmt, Args&&... args);

		template<typename... Args>
		void warn(const char* fmt, Args&&... args);

		template<typename... Args>
		void error(const char* fmt, Args&&... args);
	};

	
	template<typename ...Args>
	inline void logger::log(log_level level, const char * fmt, Args && ...args)
	{
		switch (level)
		{
		case log_level::debug:
			debug(fmt, std::forward<Args>(args)...);
			break;
		case log_level::error:
			error(fmt, std::forward<Args>(args)...);
			break;
		case log_level::info:
			info(fmt, std::forward<Args>(args)...);
			break;
		case log_level::trace:
			trace(fmt, std::forward<Args>(args)...);
			break;
		case log_level::warn:
			warn(fmt, std::forward<Args>(args)...);
			break;
		case default:
			break;
		}

	}

	template<typename ...Args>
	inline void logger::debug(const char * fmt, Args && ...args)
	{
		logger_->debug(fmt, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void logger::trace(const char * fmt, Args && ...args)
	{
		logger_->trace(fmt, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void logger::info(const char * fmt, Args && ...args)
	{
		logger_->info(fmt, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void logger::warn(const char * fmt, Args && ...args)
	{
		logger_->warn(fmt, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void logger::error(const char * fmt, Args && ...args)
	{
		logger_->error(fmt, std::forward<Args>(args)...);
	}
}