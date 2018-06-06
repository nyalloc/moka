#include "logger/logger.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>

namespace loki
{
    /**
    *Input the filename without extension
    */
    logger::logger(std::string filename)
    {
        m_oLocalTime = std::chrono::system_clock::now();

        m_oFile.open(filename, std::fstream::out | std::fstream::app | std::fstream::ate);
    }

    logger::~logger()
    {
        m_oFile.flush();
        m_oFile.close();
    }

    logstream logger::operator()()
    {
        return logstream(*this, level::info);
    }

    logstream logger::operator()(level nLevel)
    {
        return logstream(*this, nLevel);
    }

    /**
    * Log Message
    */
    void logger::log(level nLevel, std::string oMessage)
    {
        const static char* LevelStr[] = { "debug", "info", "warning", "error" };

        using std::chrono::system_clock;
        const std::time_t tt = system_clock::to_time_t(system_clock::now());

        std::string str = std::ctime(&tt);

        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());

        m_oMutex.lock();
        std::cout << '[' << str << ']' << '[' << LevelStr[static_cast<int>(nLevel)] << "]" << std::endl << oMessage << std::endl << std::endl;
        m_oFile << '[' << str << ']' << '[' << LevelStr[static_cast<int>(nLevel)] << "]" << std::endl << oMessage << std::endl << std::endl;
        m_oMutex.unlock();
    }

    logstream::logstream(logger& oLogger, level nLevel) :
        m_oLogger(oLogger), m_nLevel(nLevel)
    {}

    logstream::logstream(const logstream& ls) :
        m_oLogger(ls.m_oLogger), m_nLevel(ls.m_nLevel)
    {}

    logstream::~logstream()
    {
        m_oLogger.log(m_nLevel, str());
    }
}
