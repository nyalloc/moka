#pragma once
#include <utility>

namespace moka
{
    struct message_base
    {
        virtual ~message_base() = default;
    };

    template<typename T>
    struct wrapped_message
    {
        T content;

        template<typename... Args>
        explicit wrapped_message(Args&&... content);
    };

    template <typename T>
    template <typename ... Args>
    wrapped_message<T>::wrapped_message(Args&&... content)
        : content(std::forward<T>(content))
    {}
}
