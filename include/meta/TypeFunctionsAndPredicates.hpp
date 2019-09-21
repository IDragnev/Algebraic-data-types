#pragma once

#include "Pair.hpp"
#include <type_traits>

namespace IDragnev::Meta
{
    template <typename T>
    struct IdentityT
    {
        using type = T;
    };

    template <typename T>
    struct CloneT
    {
        using type = Pair<T, T>;
    };

    template <typename T, typename F>
    struct LargerT
    {
        using type = std::conditional_t<sizeof(T) >= sizeof(F), T, F>;
    };

    template <typename U, typename V>
    struct IsSmallerT : std::bool_constant<(sizeof(U) < sizeof(V))> { };

    template <template <typename... Args> typename Predicate>
    struct Inverse
    {
        template <typename... Args>
        struct invoke : std::bool_constant<!Predicate<Args...>::value> { };
    };
}