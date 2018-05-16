#pragma once

namespace loki
{
    template<class _Ty1, class _Ty2, class _Ty3>
    struct is_either
    {
        static constexpr bool value = false;
    };

    template<class _Ty1, class _Ty2>
    struct is_either<_Ty2, _Ty1, _Ty1>
    {
        static constexpr bool value = true;
    };

    template<class _Ty1, class _Ty2>
    struct is_either<_Ty1, _Ty2, _Ty1>
    {
        static constexpr bool value = true;
    };

    template<class _Ty1>
    struct is_either<_Ty1, _Ty1, _Ty1>
    {
        static constexpr bool value = true;
    };
}