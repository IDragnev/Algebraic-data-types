#pragma once

namespace IDragnev
{
    namespace Detail
    {
        template <typename U, typename V>
        struct HaveSameLength { };

        template <typename... Us, typename... Vs>
        struct HaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>> :
            std::bool_constant<(sizeof...(Us) == sizeof...(Vs))> { };

        template <typename U, typename V>
        inline constexpr bool haveSameLength = HaveSameLength<U, V>::value;

        template <typename U, typename V>
        using EnableIfHaveSameLength = std::enable_if_t<haveSameLength<U, V>>;

        template <typename CompareFn>
        inline constexpr
        bool compareWith(CompareFn&&, const Tuple<>&, const Tuple<>&) noexcept
        {
            return true;
        }

        template <typename CompareFn,
                  typename UHead, typename... UTail,
                  typename VHead, typename... VTail
        > constexpr bool
        compareWith(CompareFn compare, const Tuple<UHead, UTail...>& u, const Tuple<VHead, VTail...>& v)
        {
            return compare(u.getHead(), v.getHead()) &&
                           compareWith(compare, u.getTail(), v.getTail());
        }
    }

    template <typename... Types>
    inline constexpr
    auto makeTuple(Types&&... args)
    {
        using T = Tuple<std::decay_t<Types>...>;
        return T(std::forward<Types>(args)...);
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator==(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return Detail::compareWith(std::equal_to{}, u, v);
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator!=(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return !(u == v);
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator<(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return Detail::compareWith(std::less{}, u, v);
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator>(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return v < u;
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator>=(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return Detail::compareWith(std::greater_equal{}, u, v);
    }

    template <typename... Us,
              typename... Vs,
              typename = Detail::EnableIfHaveSameLength<Meta::TypeList<Us...>, Meta::TypeList<Vs...>>
    > inline constexpr
    bool operator<=(const Tuple<Us...>& u, const Tuple<Vs...>& v)
    {
        return Detail::compareWith(std::less_equal{}, u, v);
    }

    template <typename TupleT,
              typename F,
              typename = std::enable_if_t<isTuple<TupleT>>
    > inline constexpr 
    decltype(auto) operator|(TupleT&& t, F&& f)
    {
        static_assert(std::is_invocable_v<decltype(f), decltype(t)>,
                      "cannot invoke the passed function with the piped tuple");
        return std::forward<F>(f)(std::forward<TupleT>(t));
    }

    namespace Detail
    {
        struct IgnoreT
        {
            template <typename T>
            constexpr
            const IgnoreT& operator=(const T&) const noexcept { return *this; }
        };
    }

    inline constexpr Detail::IgnoreT ignore{};

    template <typename... Types>
    constexpr inline
    auto tie(Types&... args) noexcept
    {
        return Tuple<Types&...>(args...);
    }
}