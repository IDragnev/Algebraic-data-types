
namespace IDragnev::Detail
{
    template<typename T, typename... AllTypes>
    inline VariantChoice<T, AllTypes...>::VariantChoice(T&& value)
    {
        emplace(std::move(value));
    }

    template<typename T, typename... AllTypes>
    inline VariantChoice<T, AllTypes...>::VariantChoice(const T& value)
    {
        emplace(value);
    }

    template <typename T, typename... AllTypes>
    template <typename Value>
    void VariantChoice<T, AllTypes...>::emplace(Value&& value)
    {
        new(asDerived().getRawBuffer()) T(std::forward<Value>(value));
        asDerived().setDiscriminator(discriminator);
    }

    template <typename T, typename... AllTypes>
    inline auto VariantChoice<T, AllTypes...>::asDerived() noexcept -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

    template <typename T, typename... AllTypes>
    inline auto VariantChoice<T, AllTypes...>::asDerived() const noexcept ->  const Derived&
    {
        return static_cast<const Derived&>(*this);
    }

    template<typename T, typename... AllTypes>
    inline auto VariantChoice<T, AllTypes...>::operator=(T&& value) -> Derived&
    {
        return assign(std::move(value));
    }

    template <typename T, typename... AllTypes>
    inline auto VariantChoice<T, AllTypes...>::operator=(const T& value) -> Derived&
    {
        return assign(value);
    }

    template <typename T, typename... AllTypes>
    template <typename Value>
    auto VariantChoice<T, AllTypes...>::assign(Value&& value) -> Derived&
    {
        if (isTheCurrentVariantChoice())
        {
            asDerived().template get<T>() = std::forward<Value>(value);
        }
        else
        {
            asDerived().destroyValue();
            emplace(std::forward<Value>(value));
        }

        return asDerived();
    }

    template <typename T, typename... AllTypes>
    bool VariantChoice<T, AllTypes...>::isTheCurrentVariantChoice() const noexcept
    {
        return asDerived().getDiscriminator() == discriminator;
    }

    template <typename T, typename... AllTypes>
    void VariantChoice<T, AllTypes...>::destroyValueIfHoldingIt() noexcept
    {
        if (isTheCurrentVariantChoice())
        {
            asDerived().template getBufferAs<T>()->~T();
        }
    }
}