#pragma once

namespace loki
{
    template<typename... Args>
    struct size;

    template<>
    struct size<>
    {
        static constexpr size_t value = 0;
    };

    template<class T, class... Args>
    struct size<T, Args...>
    {
        static constexpr size_t value = 1 + size<Args...>::value;
    };
}