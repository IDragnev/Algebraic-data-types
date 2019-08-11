#pragma once

#include "Meta\ListAlgorithms.h"

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
        VariantChoice(const T& value);         
        
        void destroyValueIfHoldingIt() noexcept;
        
        Derived& operator=(T&& value);
        Derived& operator=(const T& value);  

    protected:
        static constexpr unsigned discriminator = Meta::indexOf<T, Meta::TypeList<Types...>> + 1;

        static bool isTheCurrentVariantChoice() noexcept;

    private:
        template <typename Value>
        void emplace(Value&& value);
        template <typename Value>
        Derived& assign(Value&& value);

        Derived& asDerived() noexcept;
        const Derived& asDerived() const noexcept;
    };
}

#include "VariantChoiceImpl.hpp"