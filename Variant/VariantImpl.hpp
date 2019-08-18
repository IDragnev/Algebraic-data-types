
#include <assert.h>

namespace IDragnev
{
    template <typename... Types>
    Variant<Types...>::Variant()
    {
        using T = Meta::Head<Meta::TypeList<Types...>>;  
        *this = T{};
    }

    template <typename... Types>
    Variant<Types...>::Variant(Variant&& source)
    {
        if (!source.isEmpty())
        {
            std::move(source).visit([this](auto&& value) 
            {
                *this = std::move(value);
            });
        }
    }

    template <typename... Types>
    template <typename... SourceTypes>
    Variant<Types...>::Variant(const Variant<SourceTypes...>& source)
    {
        if (!source.isEmpty())
        {
            source.visit([this](auto const& value) 
            {
                *this = value;
            });
        }
    }

    template <typename... Types>
    auto Variant<Types...>::operator=(const Variant& source) -> Variant&
    {
        if (!source.isEmpty())
        {
            source.visit([this](const auto& value)
            {
                *this = value;
            });
        }
        else
        {
            destroyValue();
        }

        return *this;
    }

    template <typename... Types>
    auto Variant<Types...>::operator=(Variant&& source) -> Variant&
    {
        if (!source.isEmpty())
        {
            source.visit([this](auto&& value)
            {
                *this = std::move(value);
            });
        }
        else
        {
            destroyValue();
        }

        return *this;
    }

    template <typename... Types>
    Variant<Types...>::Variant(const Variant& source)
    {
        if (!source.isEmpty())
        {
            source.visit([this](const auto& value) 
            {
                *this = value;
            });
        }
    }

    template <typename... Types>
    inline Variant<Types...>::~Variant()
    {
        destroyValue();
    }

    template<typename... Types>
    void Variant<Types...>::destroyValue() noexcept
    {
        (VariantChoice<Types, Types...>::destroyValueIfHoldingIt(), ...);
        this->setDiscriminator(NO_VALUE_DISCRIMINATOR);
    }

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
        return *(this->template getBufferAs<T>());
    }

    template<typename... Types>
    inline bool Variant<Types...>::isEmpty() const noexcept
    {
        return this->getDiscriminator() == NO_VALUE_DISCRIMINATOR;
    }

    namespace Detail 
    {
        template <typename R,
                  typename V,
                  typename Visitor,
                  typename Head,
                  typename... Tail
        > R variantVisit(V&& variant, Visitor&& visitor, Meta::TypeList<Head, Tail...>)
        {
            if (variant.template is<Head>())
            {
                return static_cast<R>(
                    std::invoke(std::forward<Visitor>(visitor),
                                std::forward<V>(variant).template get<Head>()));
            }
            else if constexpr (sizeof...(Tail) > 0)
            {
                return variantVisit<R>(std::forward<V>(variant),
                                       std::forward<Visitor>(visitor),
                                       Meta::Typelist<Tail...>{});
            }
            else
            {
                throw EmptyVariant();
            }
        }
    }

    template <typename... Types>
    template <typename R, typename Visitor>
    Detail::VisitResult<R, Visitor, Types&...>
    Variant<Types...>::visit(Visitor&& v) &
    {
        using Result = Detail::VisitResult<R, Visitor, Types&...>;
        return Detail::variantVisit<Result>(*this, 
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }

    template <typename... Types>
    template <typename R, typename Visitor>
    Detail::VisitResult<R, Visitor, const Types&...>
    Variant<Types...>::visit(Visitor&& v) const&
    {
        using Result = Detail::VisitResult<R, Visitor, const Types&...>;
        return Detail::variantVisit<Result>(*this, 
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }

    template <typename... Types>
    template <typename R, typename Visitor>
    Detail::VisitResult<R, Visitor, Types&&...>
    Variant<Types...>::visit(Visitor&& v) &&
    {
        using Result = Detail::VisitResult<R, Visitor, Types&&...>;
        return Detail::variantVisit<Result>(std::move(*this),
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }
}