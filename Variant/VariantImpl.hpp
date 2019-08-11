
#include <assert.h>

namespace IDragnev
{
    template <typename... Types>
    template <typename T>
    bool Variant<Types...>::is() const noexcept
    {
        return this->getDiscriminator() == VariantChoice<T, Types...>::discriminator;
    }

    template <typename... Types>
    template <typename T>
    T&& Variant<Types...>::get() &&
    {
        return std::move(get());
    }

    template <typename... Types>
    template <typename T>
    T& Variant<Types...>::get() &
    {
        return const_cast<T&>(std::as_const(*this).get());
    }

    template <typename... Types>
    template <typename T>
    const T& Variant<Types...>::get() const &
    {
        if (isEmpty())
        {
            throw EmptyVariant{};
        }

        assert(is<T>());
        return *this->template getBufferAs<T>();
    }
}