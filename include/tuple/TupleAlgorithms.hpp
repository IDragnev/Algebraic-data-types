#pragma once

#include "Meta/ListInterface.h"
#include "Meta/ListAlgorithms.h"
#include "Tuple.h"

namespace IDragnev
{
    template <typename H, typename... Tail>
    struct Meta::HeadT<Tuple<H, Tail...>>
    {
        using type = H;
    };

    template <>
    struct Meta::HeadT<Tuple<>> { };

    template <typename H, typename... Tail>
    struct Meta::TailT<Tuple<H, Tail...>>
    {
        using type = Tuple<Tail...>;
    };

    template <>
    struct Meta::TailT<Tuple<>> { };

    template <typename... Elems, typename... Ts>
    struct Meta::InsertFrontT<Tuple<Elems...>, Ts...>
    {
        using type = Tuple<Ts..., Elems...>;
    };

    template <>
    struct Meta::IsEmpty<Tuple<>> : std::true_type { };

    template <typename... Elems, typename... Ts>
    struct Meta::InsertBackT<Tuple<Elems...>, Ts...>
    {
        using type = Tuple<Elems..., Ts...>;
    };

    template <typename... Elems>
    struct Meta::EmptyListT<Tuple<Elems...>>
    {
        using type = Tuple<>;
    };
}

namespace IDragnev::TupleAlgorithms
{
    namespace Detail
    {
        enum class InsertionPolicy { front, back };

        template <InsertionPolicy policy,
                  typename TupleT,
                  typename... Ts,
                  std::size_t... Indices
        > inline constexpr
        auto insertAt(TupleT&& tuple,
                      Meta::ValueList<std::size_t, Indices...>,
                      Ts&&... values)
        {
            if constexpr (policy == InsertionPolicy::back)
            {
                return makeTuple(get<Indices>(std::forward<TupleT>(tuple))...,
                                 std::forward<Ts>(values)...);
            }
            else
            {
                return makeTuple(std::forward<Ts>(values)...,
                                 get<Indices>(std::forward<TupleT>(tuple))...);
            }
        }
    }

    template <typename TupleT,
              typename T,
              typename... Rest,
              std::size_t Size = tupleSize<TupleT>
    > inline constexpr
    auto insertFront(TupleT&& tuple, T&& value, Rest&&... rest)
    {
        using IP = Detail::InsertionPolicy;
        using Indices = Meta::MakeIndexList<Size>;

        return Detail::insertAt<IP::front>(std::forward<TupleT>(tuple),
                                           Indices{},
                                           std::forward<T>(value),
                                           std::forward<Rest>(rest)...);
    }

    template <typename TupleT,
              typename T,
              typename... Rest,
              std::size_t Size = tupleSize<TupleT>
    > inline constexpr
    auto insertBack(TupleT&& tuple, T&& value, Rest&&... rest)
    {
        using IP = Detail::InsertionPolicy;
        using Indices = Meta::MakeIndexList<Size>;

        return Detail::insertAt<IP::back>(std::forward<TupleT>(tuple),
                                          Indices{},
                                          std::forward<T>(value),
                                          std::forward<Rest>(rest)...);
    }

    namespace Detail
    {
        template <typename TupleT, std::size_t... Indices>
        inline constexpr
        auto select(TupleT&& tuple, Meta::ValueList<std::size_t, Indices...>)
        {
            return makeTuple(get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <std::size_t... Indices>
    struct Select 
    {
        template <typename TupleT,
                  typename = std::enable_if_t<isTuple<TupleT>>
        > inline constexpr
        auto operator()(TupleT&& tuple) const
        {
            using IndicesT = Meta::ValueList<std::size_t, Indices...>;
            return Detail::select(std::forward<TupleT>(tuple), IndicesT{});
        }
    };

    template <std::size_t... Indices>
    inline constexpr auto select = Select<Indices...>{};

    struct Reverse
    {
        template <typename TupleT,
                  std::size_t Size = tupleSize<TupleT>
        > inline constexpr
        auto operator()(TupleT&& tuple) const
        {
            using Meta::MakeIndexList;
            using Indices = Meta::Reverse<MakeIndexList<Size>>;

            return Detail::select(std::forward<TupleT>(tuple), Indices{});
        }
    };

    inline constexpr auto reverse = Reverse{};

    template <std::size_t Index, std::size_t Count>
    struct Replicate
    {
        template <typename Head,
                  typename... Tail
        > inline constexpr
        auto operator()(const Tuple<Head, Tail...>& t) const
        {
            using Indices = Meta::ReplicateValue<Index, Count>;
            return Detail::select(t, Indices{});
        }
    };

    template <std::size_t Index, std::size_t Count>
    inline constexpr auto replicate = Replicate<Index, Count>{};

    template <std::size_t N>
    struct Take
    {
        template <typename TupleT,
                  typename = std::enable_if_t<isTuple<TupleT>>
        > inline constexpr
        auto operator()(TupleT&& t) const
        {
            using Indices = Meta::MakeIndexList<N>;
            return Detail::select(std::forward<TupleT>(t), Indices{});
        }
    };

    template <std::size_t N>
    inline constexpr auto take = Take<N>{};

    inline constexpr auto dropTail = take<1>;

    template <std::size_t N>
    struct Drop
    {
        template <typename TupleT,
                  std::size_t Size = tupleSize<TupleT>
        > inline constexpr
        auto operator()(TupleT&& t) const
        {
            using Meta::MakeIndexList;
            using Indices = Meta::Drop<N, MakeIndexList<Size>>;

            return Detail::select(std::forward<TupleT>(t), Indices{});
        }
    };

    template <std::size_t N>
    inline constexpr auto drop = Drop<N>{};

    inline constexpr auto dropHead = drop<1>;

    template <template <typename U, typename V> typename CompareFn>
    struct SortByType
    {
        template <typename TupleT,
                  std::size_t Size = tupleSize<TupleT>
        > constexpr 
        auto operator()(TupleT&& t) const
        {
            using Meta::InsertionSort;
            using Meta::MakeIndexedCompareT;
            using TypeList = std::decay_t<TupleT>;
            using InitialIndices = Meta::MakeIndexList<Size>;
            using SortedIndices = InsertionSort<InitialIndices,
                                                MakeIndexedCompareT<TypeList, CompareFn>::template invoke>;

            return Detail::select(std::forward<TupleT>(t), SortedIndices{});
        }
    };

    template <template <typename U, typename V> typename CompareFn>
    inline constexpr auto sortByType = SortByType<CompareFn>{};

    namespace Detail
    {
        template <typename Callable, typename... Args>
        constexpr void forEachArg(Callable f, Args&&... args)
        {
            (f(std::forward<Args>(args)), ...);
        }

        template <typename Callable,
                  typename TupleT,
                  std::size_t... Indices
        > inline constexpr
        void forEach(TupleT&& tuple, Callable f, Meta::ValueList<std::size_t, Indices...>)
        {
            forEachArg(f, get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <typename Callable,
              typename TupleT,
              std::size_t Size = tupleSize<TupleT>
    > inline constexpr
    void forEach(TupleT&& tuple, Callable f)
    {
        using Indices = Meta::MakeIndexList<Size>;
        Detail::forEach(std::forward<TupleT>(tuple), f, Indices{});
    }

    namespace Detail
    {
        template <typename Callable,
                  typename TupleT,
                  std::size_t... Indices
        > inline constexpr
        decltype(auto) apply(Callable f, TupleT&& tuple, Meta::ValueList<std::size_t, Indices...>)
        {
            return f(get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <typename Callable,
              typename TupleT,
              std::size_t Size = tupleSize<TupleT>
    > inline constexpr
    decltype(auto) apply(Callable f, TupleT&& tuple)
    {
        using Indices = Meta::MakeIndexList<Size>;
        return Detail::apply(f, std::forward<TupleT>(tuple), Indices{});
    }

    template <typename TupleT,
              typename T,
              typename BinaryOp,
              typename = std::enable_if_t<isTuple<TupleT> && 
                                          Meta::isEmpty<std::decay_t<TupleT>>>
    > inline constexpr
    T foldl(TupleT&&, T&& acc, BinaryOp)
    {
        return std::forward<T>(acc);
    }

    template <typename TupleT,
              typename T,
              typename BinaryOp,
              typename = std::enable_if_t<isTuple<TupleT> && 
                                          !Meta::isEmpty<std::decay_t<TupleT>>>
    > constexpr
    decltype(auto) foldl(TupleT&& tuple, T&& acc, BinaryOp op)
    {
        return foldl(std::forward<TupleT>(tuple).getTail(),
                     op(std::forward<T>(acc), std::forward<TupleT>(tuple).getHead()),
                     op);
    }

    namespace Detail 
    {
        template <typename TupleT,
                  typename UnaryFunction,
                  std::size_t... Indices
        > inline constexpr
        auto transform(TupleT&& tuple, UnaryFunction f, Meta::ValueList<std::size_t, Indices...>)
        {
            return makeTuple(f(get<Indices>(std::forward<TupleT>(tuple)))...);
        }
    }

    template <typename TupleT,
              typename UnaryFunction,
              std::size_t Size = tupleSize<TupleT>
    > inline constexpr
    auto transform(TupleT&& tuple, UnaryFunction f)
    {
        using Indices = Meta::MakeIndexList<Size>;
        return Detail::transform(std::forward<TupleT>(tuple), f, Indices{});
    }

    namespace Detail
    {
        template <typename UTuple,
                  typename VTuple,
                  std::size_t... UIndices,
                  std::size_t... VIndices
        > constexpr auto concatenate(UTuple&& u, VTuple&& v,
                                     Meta::ValueList<std::size_t, UIndices...>,
                                     Meta::ValueList<std::size_t, VIndices...>)
        {
            return makeTuple(get<UIndices>(std::forward<UTuple>(u))...,
                             get<VIndices>(std::forward<VTuple>(v))...);
        }

        template <typename... Types>
        inline constexpr bool areAllTuples = Meta::allOf<IDragnev::Detail::IsTuple, 
                                                         Meta::TypeList<std::decay_t<Types>... >> ;
    }

    template <typename UTuple,
              typename VTuple,
              std::size_t USize = tupleSize<UTuple>,
              std::size_t VSize = tupleSize<VTuple>
    > constexpr auto concatenate(UTuple&& u, VTuple&& v)
    {
        using UIndices = Meta::MakeIndexList<USize>;
        using VIndices = Meta::MakeIndexList<VSize>;

        return Detail::concatenate(std::forward<UTuple>(u),
                                   std::forward<VTuple>(v),
                                   UIndices{},
                                   VIndices{});
    }

    template <typename UTuple,
              typename VTuple,
              typename... Tuples,
              typename = std::enable_if_t<Detail::areAllTuples<UTuple, VTuple, Tuples...>>
    > constexpr auto concatenate(UTuple&& u, VTuple&& v, Tuples&&... rest)
    {
        return concatenate(concatenate(std::forward<UTuple>(u),
                           std::forward<VTuple>(v)),
                           std::forward<Tuples>(rest)...);
    }
}