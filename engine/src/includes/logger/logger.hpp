#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <asset_importer/filesystem.hpp>
#include <windows.h>
#include <wincon.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <mutex>

using namespace std::experimental;

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

    enum class level { debug, trace, info, warning, error, fatal };

    class logger
    {
        static std::mutex mutex_; 
        constexpr static int light_green_ = 10;
        constexpr static int light_cyan_ = 11;
        constexpr static int light_red_ = 12;
        constexpr static int light_magenta_ = 13;
        constexpr static int yellow_ = 14;
        constexpr static int white_ = 15;
        bool print_to_console_;
		fs::path log_path_;

        template<typename T>
        static void log(std::ostream& file, const level level, const T& message) 
        {
            const auto h_console = GetStdHandle(STD_OUTPUT_HANDLE);

            switch (level)
            {
            case level::debug:
                if constexpr(application_traits::is_debug_build)
                {
                    SetConsoleTextAttribute(h_console, white_);
                    print(file, "[debug]", message);
                }
                break;
            case level::trace:
                SetConsoleTextAttribute(h_console, light_cyan_);
                print(file, "[trace]", message);
                break;
            case level::info:
                SetConsoleTextAttribute(h_console, light_green_);
                print(file, "[info]", message);
                break;
            case level::warning:
                SetConsoleTextAttribute(h_console, yellow_);
                print(file, "[warning]", message);
                break;
            case level::error:
                SetConsoleTextAttribute(h_console, light_red_);
                print(file, "[error]", message);
                break;
            case level::fatal:
                SetConsoleTextAttribute(h_console, light_magenta_);
                print(file, "[fatal]", message);
                break;
            default:;
            }

            SetConsoleTextAttribute(h_console, white_);
        }
    public:

        logger(const fs::path& log_path, const bool print_to_console = true)
            : print_to_console_(print_to_console), log_path_(log_path)
        {
        }

        ~logger()
        {
        }

        static std::string get_date()
        {
            using namespace std::chrono;

            auto now = system_clock::now();
            auto timer = system_clock::to_time_t(now);
            std::tm bt = *std::localtime(&timer);
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

            std::ostringstream oss;

            oss << std::put_time(&bt, "%Y-%m-%d") << " ";
            oss << std::put_time(&bt, "%T");
            oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

            return oss.str();
        }

        template<typename T>
        static void print(std::ostream& file, std::string&& prefix, const T& message, const bool print_to_console = true)
        {
            std::stringstream result;

            prefix += std::string{ '[' + get_date() + "]: " };

            std::stringstream ss;
            ss << message;
            std::string to;

            std::vector<std::string> lines;

            while (getline(ss, to, '\n')) 
            {
                lines.emplace_back(to);
            }

			file << prefix << lines[0] << std::endl;
			if (print_to_console)
			{
				std::cout << prefix << lines[0] << std::endl;
			}

            if (lines.size() > 1)
            {
                for (size_t i = 1; i < lines.size(); i++)
                {
                    file << std::string(prefix.size(), ' ') << lines[i] << std::endl;
                    if (print_to_console)
                    {
                        std::cout << std::string(prefix.size(), ' ') << lines[i] << std::endl;
                    }
                }
            }
        }

        template<typename T>
        void log(const level level, const T& message) const
        {
			std::unique_lock<std::mutex> lock(mutex_);
			std::ofstream file;
			file.open(log_path_, std::ofstream::app);
            log(file, level, message);
			file.close();
        }
    };
}

#endif