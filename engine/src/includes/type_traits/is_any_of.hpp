#pragma once

namespace neon
{
    template<class _Ty1, class _Ty2, typename... Args>
    struct is_any_of
    {
        static constexpr bool value = is_any_of<_Ty1, _Ty2>::value || is_any_of<_Ty1, Args...>::value;
    };

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
