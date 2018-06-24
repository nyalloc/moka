
#include <mutex>
#include "logger/logger.hpp"

namespace neon
{
    std::mutex logger::mutex_ = {};
}