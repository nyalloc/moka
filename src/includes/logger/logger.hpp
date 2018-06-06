#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <mutex>
#include <fstream>

namespace loki
{
    enum class level { debug, info, warning, error };

    class logger;

    class logstream : public std::ostringstream
    {
    public:
        logstream(logger& oLogger, level nLevel);
        logstream(const logstream& ls);
        ~logstream();

    private:
        logger & m_oLogger;
        level m_nLevel;
    };

    class logger
    {
    public:
        logger(std::string filename);
        virtual ~logger();

        logstream operator()();
        logstream operator()(level nLevel);
        void log(level nLevel, std::string oMessage);

    private:
        std::chrono::system_clock::time_point m_oLocalTime;

        std::mutex m_oMutex;
        std::ofstream m_oFile;

    };
}

#endif