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
        
        void destroyValueIfHoldingIt() noexcept;
        
        Derived& operator=(const T& value);  
        Derived& operator=(T&& value);

    protected:
        static constexpr unsigned discriminator = Meta::indexOf<T, Meta::TypeList<Types...>> + 1;

        static bool isTheCurrentVariantChoice() noexcept;

    private:
        template <typename Value>
        void emplace(Value&& value);
        template <typename Value>
        Derived& assign(Value&& value);

        Derived& asDerived() noexcept;
        Derived const& asDerived() const noexcept;
    };
}

#include "VariantChoiceImpl.hpp"