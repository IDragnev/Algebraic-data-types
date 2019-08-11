
namespace IDragnev::Detail
{
    template<typename T, typename... Types>
    VariantChoice<T, Types...>::VariantChoice(T&& value)
    {
        constructWith(std::move(value));
    }

    template<typename T, typename... Types>
    VariantChoice<T, Types...>::VariantChoice(const T& value)
    {
        constructWith(value);
    }

    template <typename T, typename... Types>
    template <typename Value>
    void VariantChoice<T, Types...>::constructWith(Value&& value)
    {
        new(asDerived().getRawBuffer()) T(std::forward<Value>(value));
        asDerived().setDiscriminator(discriminator);
    }

    template <typename T, typename... Types>
    inline auto VariantChoice<T, Types...>::asDerived() noexcept -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

    template <typename T, typename... Types>
    inline auto VariantChoice<T, Types...>::asDerived() const noexcept ->  const Derived&
    {
        return static_cast<const Derived&>(*this);
    }

    template <typename T, typename... Types>
    void VariantChoice<T, Types...>::destroyValueIfHoldingIt() noexcept
    {
        if (asDerived().getDiscriminator() == discriminator)
        {
            asDerived().template getBufferAs<T>()->~T();
        }
    }
}