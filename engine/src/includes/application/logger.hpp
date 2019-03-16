#pragma once

#include <spdlog/spdlog.h>

namespace moka
{
    /**
     * \brief A wrapper around compile-time constants that can be used for conditional compilation in the application.
     */
    struct application_traits final
    {
#ifdef NDEBUG
        constexpr static bool is_debug_build = false; /**< Is this build of moka a release build or a debug build? */
#else
        constexpr static bool is_debug_build = true; /**< Is this build of moka a release build or a debug build? */
#endif
    };

    /**
     * \brief The severity of the item being logged
     */
    enum class log_level
    {
        trace, /**< Designates finer-grained informational events than log_level::debug. */
        debug, /**< Designates fine-grained informational events that are most useful to debug an application. */
        info, /**< 	Designates informational messages that highlight the progress of the application at coarse-grained level. */
        warn, /**< Designates potentially harmful situations. */
        error, /**< Designates error events that might still allow the application to continue running. */
    };

    /**
     * \brief Get the default log level of the application.
     * \return If the application is a debug build, log_level::debug. Otherwise, log_level::info.
     */
    constexpr log_level default_log_level()
    {
        return application_traits::is_debug_build ? log_level::debug : log_level::info;
    }

    /**
     * \brief A logging class. A simple wrapper around a spdlog::logger object.
     */
    class logger final
    {
        std::shared_ptr<spdlog::logger> logger_;

    public:
        /**
         * \brief Create a new logger object.
         * \param name The name of this logger.
         * \param level The log level of this object. Items logged must be higher than this level to be saved to the log.
         */
        explicit logger(const char* name, log_level level = default_log_level());

        /**
         * \brief Write some information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param level The log level that the arguments will be logged at.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void log(log_level level, const char* fmt, Args&&... args);

        /**
         * \brief Write some debug-level information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void debug(const char* fmt, Args&&... args);

        /**
         * \brief Write some trace-level information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void trace(const char* fmt, Args&&... args);

        /**
         * \brief Write some info-level information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void info(const char* fmt, Args&&... args);

        /**
         * \brief Write some warn-level information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void warn(const char* fmt, Args&&... args);

        /**
         * \brief Write some error-level information to the log.
         * \tparam Args A parameter pack of arguments that are being written to the log.
         * \param fmt The format of the arguments.
         * \param args The arguments that are being written to the log.
         */
        template <typename... Args>
        void error(const char* fmt, Args&&... args);
    };

    inline logger::logger(const char* name, const log_level level)
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

    template <typename... Args>
    void logger::log(const log_level level, const char* fmt, Args&&... args)
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
        default:
            break;
        }
    }

    template <typename... Args>
    void logger::debug(const char* fmt, Args&&... args)
    {
        logger_->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logger::trace(const char* fmt, Args&&... args)
    {
        logger_->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logger::info(const char* fmt, Args&&... args)
    {
        logger_->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logger::warn(const char* fmt, Args&&... args)
    {
        logger_->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logger::error(const char* fmt, Args&&... args)
    {
        logger_->error(fmt, std::forward<Args>(args)...);
    }
} // namespace moka
