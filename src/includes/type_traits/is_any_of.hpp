#pragma once

namespace loki
{
    namespace detail
    {
        template<class _Ty1, class _Ty2>
        struct is_any_of<_Ty1, _Ty2>
        {
            static constexpr bool value = false;
        };

        template<class _Ty1>
        struct is_any_of<_Ty1, _Ty1>
        {
            static constexpr bool value = true;
        };
    }

    template<class _Ty1, class _Ty2, typename... Args>
    struct is_any_of
    {
        static constexpr bool value = detail::is_any_of<_Ty1, _Ty2>::value || is_any_of<_Ty1, Args...>::value;
    };
}
