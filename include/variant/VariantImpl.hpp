
#include <assert.h>
#include <functional>

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
        copyFromIfNotEmpty(std::move(source));
    }

    template <typename... Types>
    template <typename... SourceTypes>
    Variant<Types...>::Variant(Variant<SourceTypes...>&& source)
    {
        copyFromIfNotEmpty(std::move(source));
    }

    template <typename... Types>
    Variant<Types...>::Variant(const Variant& source)
    {
        copyFromIfNotEmpty(source);
    }

    template <typename... Types>
    template <typename... SourceTypes>
    Variant<Types...>::Variant(const Variant<SourceTypes...>& source)
    {
        copyFromIfNotEmpty(source);
    }

    template <typename... Types>
    template <typename VariantT>
    inline void Variant<Types...>::copyFromIfNotEmpty(VariantT&& source)
    {
        if (!source.isEmpty())
        {
            copyFrom(std::forward<VariantT>(source));
        }
    }

    template <typename... Types>
    template <typename VariantT>
    void Variant<Types...>::copyFrom(VariantT&& source)
    {
        assert(!source.isEmpty());
        visit(std::forward<VariantT>(source), [this](auto&& value)
        {
            *this = std::forward<decltype(value)>(value);
        });
    }

    template<typename... Types>
    inline bool Variant<Types...>::isEmpty() const noexcept
    {
        return this->getDiscriminator() == NO_VALUE_DISCRIMINATOR;
    }

    template <typename... Types>
    Variant<Types...>::~Variant()
    {
        destroyValue();
    }

    template<typename... Types>
    void Variant<Types...>::destroyValue() noexcept
    {
        (VChoice<Types>::destroyValueIfHoldingIt(), ...);
        this->setDiscriminator(NO_VALUE_DISCRIMINATOR);
    }

    template <typename... Types>
    inline auto Variant<Types...>::operator=(Variant&& source) -> Variant&
    {
        assignFrom(std::move(source));
        return *this;
    }

    template <typename... Types>
    template <typename... SourceTypes>
    inline auto Variant<Types...>::operator=(Variant<SourceTypes...>&& source) -> Variant&
    {
        assignFrom(std::move(source));
        return *this;
    }

    template <typename... Types>
    inline auto Variant<Types...>::operator=(const Variant& source) -> Variant&
    {
        assignFrom(source);
        return *this;
    }

    template <typename... Types>
    template <typename... SourceTypes>
    inline auto Variant<Types...>::operator=(const Variant<SourceTypes...>& source) -> Variant&
    {
        assignFrom(source);
        return *this;
    }

    template <typename... Types>
    template <typename VariantT>
    void Variant<Types...>::assignFrom(VariantT&& source)
    {
        if (!source.isEmpty())
        {
            copyFrom(std::forward<VariantT>(source));
        }
        else
        {
            destroyValue();
        }
    }

    template <typename... Types>
    template <typename T>
    bool Variant<Types...>::is() const noexcept
    {
        return this->getDiscriminator() == VChoice<T>::discriminator;
    }

    template <typename... Types>
    template <typename T>
    inline T&& Variant<Types...>::get() &&
    {
        return std::move(get<T>());
    }

    template <typename... Types>
    template <typename T>
    inline T& Variant<Types...>::get() &
    {
        return const_cast<T&>(std::as_const(*this).template get<T>());
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
                                       Meta::TypeList<Tail...>{});
            }
            else
            {
                throw EmptyVariant{};
            }
        }
    }

    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, Types&...> 
    visit(Variant<Types...>& variant, Visitor&& v)
    {
        using Result = Detail::VisitResult<R, Visitor, Types&...>;
        return Detail::variantVisit<Result>(variant,
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }
        
    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, const Types&...>
    visit(const Variant<Types...>& variant, Visitor&& v)
    {
        using Result = Detail::VisitResult<R, Visitor, const Types&...>;
        return Detail::variantVisit<Result>(variant, 
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }
        
    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, Types&&...>
    visit(Variant<Types...>&& variant, Visitor&& v)
    {
        using Result = Detail::VisitResult<R, Visitor, Types&&...>;
        return Detail::variantVisit<Result>(std::move(variant),
                                            std::forward<Visitor>(v),
                                            Meta::TypeList<Types...>{});
    }
}