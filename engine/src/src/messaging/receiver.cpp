#include <messaging/receiver.hpp>

namespace moka
{
    receiver::operator sender()
    {
        return sender{&_q};
    }

    dispatcher receiver::wait()
    {
        return dispatcher{&_q};
    }
}
