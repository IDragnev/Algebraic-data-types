#pragma once

#include "ListInterface.h"

namespace IDragnev::Meta
{
    template <typename... Elements>
    struct TypeList { };

    template <typename H, typename... Tail>
    struct HeadT<TypeList<H, Tail...>>
    {
        using type = H;
    };

    template <>
    struct HeadT<TypeList<>> { };

    template <typename T, typename... Ts>
    struct TailT<TypeList<T, Ts...>>
    {
        using type = TypeList<Ts...>;
    };

    template <typename... Elems, typename... Ts>
    struct InsertFrontT<TypeList<Elems...>, Ts...>
    {
        using type = TypeList<Ts..., Elems...>;
    };

    template <>
    struct IsEmpty<TypeList<>> : std::true_type { };

    template <typename... Elems, typename... Ts>
    struct InsertBackT<TypeList<Elems...>, Ts...>
    {
        using type = TypeList<Elems..., Ts...>;
    };

    template <typename... Elems>
    struct EmptyListT<TypeList<Elems...>>
    {
        using type = TypeList<>;
    };
}