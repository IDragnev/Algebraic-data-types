#pragma once

#include "Meta\List algorithms.h"

namespace IDragnev
{
    template <typename... Types>
    class Variant;
}

namespace IDragnev::Detail
{
    template <typename T, typename... Types>
    class VariantChoice
    {
    private:
        using Derived = Variant<Types...>;

    public:
        VariantChoice() = default;
        VariantChoice(T&& value);              
        VariantChoice(T const& value);         
        
        bool destroy();                        
        
        Derived& operator=(const T& value);  
        Derived& operator=(T&& value);

    protected:
        static constexpr unsigned discriminator = Meta::indexOf<T, Meta::TypeList<Types...>> + 1;

    private:
        Derived& asDerived() noexcept;
        Derived const& asDerived() const noexcept;
    };

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
}