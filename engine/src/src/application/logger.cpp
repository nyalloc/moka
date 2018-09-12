
#include <mutex>

#include <application/logger.hpp>

namespace moka
{
    std::mutex logger::mutex_ = {};
}