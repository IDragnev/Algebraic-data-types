
namespace IDragnev
{
    template <typename Head, typename... Tail>
    template <typename VHead, typename... VTail, typename>
    constexpr Tuple<Head, Tail...>::Tuple(VHead&& head, VTail&&... tail) :
        HeadElement(std::forward<VHead>(head)),
        TailTuple(std::forward<VTail>(tail)...)
    {
    }

    template <typename Head, typename... Tail>
    template <typename VHead, typename... VTail, typename>
    constexpr Tuple<Head, Tail...>::Tuple(const Tuple<VHead, VTail...>& source) :
        HeadElement(source.getHead()),
        TailTuple(source.getTail())
    {
    }

    template <typename Head, typename... Tail>
    template <typename VHead, typename... VTail, typename>
    constexpr Tuple<Head, Tail...>::Tuple(Tuple<VHead, VTail...>&& source) :
        HeadElement(std::move(source.getHead())),
        TailTuple(std::move(source.getTail()))
    {
    }

    template <typename Head, typename... Tail>
    template <typename VHead, typename... VTail, typename>
    constexpr auto Tuple<Head, Tail...>::operator=(const Tuple<VHead, VTail...>& rhs) -> Tuple&
    {
        getHead() = rhs.getHead();
        getTail() = rhs.getTail();

        return *this;
    }

    template <typename Head, typename... Tail>
    template <typename VHead, typename... VTail, typename>
    constexpr auto Tuple<Head, Tail...>::operator=(Tuple<VHead, VTail...>&& rhs) -> Tuple&
    {
        getHead() = std::move(rhs.getHead());
        getTail() = std::move(rhs.getTail());

        return *this;
    }

    template <typename Head, typename... Tail>
    inline constexpr
    Head&& Tuple<Head, Tail...>::getHead() && noexcept
    {
        return std::move(getHead());
    }

    template <typename Head, typename... Tail>
    inline constexpr
    Head& Tuple<Head, Tail...>::getHead() & noexcept
    {
        return const_cast<Head&>(std::as_const(*this).getHead());
    }

    template <typename Head, typename... Tail>
    inline constexpr
    const Head& Tuple<Head, Tail...>::getHead() const& noexcept
    {
        return static_cast<const HeadElement&>(*this).get();
    }

    template <typename Head, typename... Tail>
    inline constexpr
    auto Tuple<Head, Tail...>::getTail() && noexcept -> TailTuple&&
    {
        return std::move(getTail());
    }

    template <typename Head, typename... Tail>
    inline constexpr
    auto Tuple<Head, Tail...>::getTail() & noexcept -> TailTuple&
    {
        return const_cast<TailTuple&>(std::as_const(*this).getTail());
    }

    template <typename Head, typename... Tail>
    inline constexpr
    auto Tuple<Head, Tail...>::getTail() const& noexcept -> const TailTuple&
    {
        return *this;
    }
}