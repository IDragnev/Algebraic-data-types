#pragma once

#include <type_traits>

namespace IDragnev::Meta
{
    template <typename List>
    struct HeadT;

    template <typename List>
    using Head = typename HeadT<List>::type;

    template <typename List>
    struct TailT;

    template <typename List>
    using Tail = typename TailT<List>::type;

    template <typename List, typename... Ts>
    struct InsertFrontT;

    template <typename List, typename... Ts>
    using InsertFront = typename InsertFrontT<List, Ts...>::type;

    template <typename List>
    struct IsEmpty : std::false_type { };

    template <typename List>
    inline constexpr auto isEmpty = IsEmpty<List>::value;

    template <typename L, typename... Ts>
    struct InsertBackT;

    template <typename List, typename... Ts>
    using InsertBack = typename InsertBackT<List, Ts...>::type;

    template <typename List>
    struct EmptyListT;

    template <typename List>
    using EmptyList = typename EmptyListT<List>::type;
}