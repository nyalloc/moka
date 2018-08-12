
#include <mutex>
#include "logger/logger.hpp"

namespace moka
{
    std::mutex logger::mutex_ = {};
}