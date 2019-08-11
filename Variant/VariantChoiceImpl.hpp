
namespace IDragnev::Detail
{
    template<typename T, typename... Types>
    VariantChoice<T, Types...>::VariantChoice(T&& value)
    {
        emplace(std::move(value));
    }

    template<typename T, typename... Types>
    VariantChoice<T, Types...>::VariantChoice(const T& value)
    {
        emplace(value);
    }

    template <typename T, typename... Types>
    template <typename Value>
    void VariantChoice<T, Types...>::emplace(Value&& value)
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

    template<typename T, typename... Types>
    inline auto VariantChoice<T, Types...>::operator=(T&& value) -> Derived&
    {
        return assign(std::move(value));
    }

    template <typename T, typename... Types>
    inline auto VariantChoice<T, Types...>::operator=(const T& value) -> Derived&
    {
        return assign(value);
    }

    template <typename T, typename... Types>
    template <typename Value>
    auto VariantChoice<T, Types...>::assign(Value&& value) -> Derived&
    {
        if (isTheCurrentVariantChoice())
        {
            asDerived().get<T>() = std::forward<Value>(value);
        }
        else
        {
            asDerived().destroyValue();
            emplace(value);
        }

        return asDerived();
    }

    template <typename T, typename... Types>
    bool VariantChoice<T, Types...>::isTheCurrentVariantChoice() noexcept
    {
        return asDerived().getDiscriminator() == discriminator;
    }

    template <typename T, typename... Types>
    void VariantChoice<T, Types...>::destroyValueIfHoldingIt() noexcept
    {
        if (isTheCurrentVariantChoice())
        {
            asDerived().template getBufferAs<T>()->~T();
        }
    }
}