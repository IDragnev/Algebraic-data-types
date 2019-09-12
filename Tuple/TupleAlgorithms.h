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
                  unsigned... Indices
        > inline constexpr
        auto insertAt(TupleT&& tuple,
                      Meta::ValueList<unsigned, Indices...>,
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
              unsigned Size = tupleSize<TupleT>
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
              unsigned Size = tupleSize<TupleT>
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

    template <typename H, typename... Elements>
    inline constexpr
    Tuple<Elements...> dropHead(const Tuple<H, Elements...>& tuple)
    {
        return tuple.getTail();
    }

    template <typename H, typename... Elements>
    inline constexpr
    Tuple<H> dropTail(const Tuple<H, Elements...>& tuple)
    {
        return Tuple<H>(tuple.getHead());
    }

    namespace Detail
    {
        template <typename TupleT, unsigned... Indices>
        inline constexpr
        auto select(TupleT&& tuple, Meta::ValueList<unsigned, Indices...>)
        {
            return makeTuple(get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <unsigned... Indices,
              typename TupleT,
              typename = std::enable_if_t<isTuple<TupleT>>
    > inline constexpr
    auto select(TupleT&& tuple)
    {
        using IndicesT = Meta::ValueList<unsigned, Indices...>;
        return Detail::select(std::forward<TupleT>(tuple), IndicesT{});
    }

    template <typename TupleT,
              unsigned Size = tupleSize<TupleT>
    > inline constexpr
    auto reverse(TupleT&& tuple)
    {
        using Meta::Reverse;
        using Meta::MakeIndexList;
        using Indices = Reverse<MakeIndexList<Size>>;

        return Detail::select(std::forward<TupleT>(tuple), Indices{});
    }

    template <unsigned Index,
              unsigned Count,
              typename Head,
              typename... Tail
    > inline constexpr
    auto replicate(const Tuple<Head, Tail...>& t)
    {
        using Indices = Meta::ReplicateValue<Index, Count>;
        return Detail::select(t, Indices{});
    }

    template <unsigned N>
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

    template <unsigned N>
    inline constexpr auto take = Take<N>{};

    template <unsigned N,
              typename TupleT,
              unsigned Size = tupleSize<TupleT>
    > inline constexpr
    auto drop(TupleT&& t)
    {
        using Meta::Drop;
        using Meta::MakeIndexList;
        using Indices = Drop<N, MakeIndexList<Size>>;

        return Detail::select(std::forward<TupleT>(t), Indices{});
    }

    template <template <typename U, typename V> typename CompareFn,
              typename TupleT,
              unsigned Size = tupleSize<TupleT>
    > inline constexpr
    auto sortByType(TupleT&& t)
    {
        using Meta::InsertionSort;
        using Meta::MakeIndexedCompareT;
        using TypeList = std::decay_t<TupleT>;
        using InitialIndices = Meta::MakeIndexList<Size>;
        using SortedIndices = InsertionSort<InitialIndices,
                                            MakeIndexedCompareT<TypeList, CompareFn>::template invoke>;

        return Detail::select(std::forward<TupleT>(t), SortedIndices{});
    }

    namespace Detail
    {
        template <typename Callable, typename... Args>
        constexpr void forEachArg(Callable f, Args&&... args)
        {
            (f(std::forward<Args>(args)), ...);
        }

        template <typename Callable,
                  typename TupleT,
                  unsigned... Indices
        > inline constexpr
        void forEach(TupleT&& tuple, Callable f, Meta::ValueList<unsigned, Indices...>)
        {
            forEachArg(f, get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <typename Callable,
              typename TupleT,
              unsigned Size = tupleSize<TupleT>
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
                  unsigned... Indices
        > inline constexpr
        decltype(auto) apply(Callable f, TupleT&& tuple, Meta::ValueList<unsigned, Indices...>)
        {
            return f(get<Indices>(std::forward<TupleT>(tuple))...);
        }
    }

    template <typename Callable,
              typename TupleT,
              unsigned Size = tupleSize<TupleT>
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
                  unsigned... Indices
        > inline constexpr
        auto transform(TupleT&& tuple, UnaryFunction f, Meta::ValueList<unsigned, Indices...>)
        {
            return makeTuple(f(get<Indices>(std::forward<TupleT>(tuple)))...);
        }
    }

    template <typename TupleT,
              typename UnaryFunction,
              unsigned Size = tupleSize<TupleT>
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
                  unsigned... UIndices,
                  unsigned... VIndices
        > constexpr auto concatenate(UTuple&& u, VTuple&& v,
                                     Meta::ValueList<unsigned, UIndices...>,
                                     Meta::ValueList<unsigned, VIndices...>)
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
              unsigned USize = tupleSize<UTuple>,
              unsigned VSize = tupleSize<VTuple>
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