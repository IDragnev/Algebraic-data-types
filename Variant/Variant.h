#pragma once

#include "VariantChoice.h"
#include "VariantStorage.h"
#include "Meta\ListAlgorithms.h"
#include "VisitResult.h"

#include <stdexcept>

namespace IDragnev
{
    class EmptyVariant : public std::exception { };

    template <typename... Types>
    class Variant
        : private Detail::VariantStorage<Types...>,
          private Detail::VariantChoice<Types, Types...>...
    {
    private:
        template <typename T, typename... Types>
        friend class VariantChoice;

    public:
        using VariantChoice<Types, Types...>::VariantChoice...;

        Variant();                                         
        Variant(Variant&& source);                   
        Variant(const Variant& source);                    
        ~Variant();
        
        template<typename... SourceTypes>
        Variant(Variant<SourceTypes...>&& source);

        template <typename... SourceTypes>
        Variant(const Variant<SourceTypes...>& source);
        
        using VariantChoice<Types, Types...>::operator=...;

        Variant& operator=(Variant&& source);
        Variant& operator=(const Variant& source);

        template<typename... SourceTypes>
        Variant& operator=(Variant<SourceTypes...>&& source);

        template<typename... SourceTypes>
        Variant& operator=(const Variant<SourceTypes...>& source);

        template <typename T>
        bool is() const noexcept;          
        
        template <typename T> 
        T& get() &;

        template <typename T> 
        T&& get() &&;

        template <typename T> 
        const T& get() const&;      

        template <typename R = Detail::ComputeResultType,
                  typename Visitor
        > Detail::VisitResult<R, Visitor, Types&...> 
        visit(Visitor&& v) &;
        
        template <typename R = Detail::ComputeResultType,
                  typename Visitor
        > Detail::VisitResult<R, Visitor, const Types&...>
        visit(Visitor&& v) const&;
        
        template <typename R = Detail::ComputeResultType,
                  typename Visitor
        > Detail::VisitResult<R, Visitor, Types&&...>
        visit(Visitor&& v) &&;

        bool isEmpty() const noexcept;

    private:
        static constexpr unsigned char NO_VALUE_DISCRIMINATOR = 0;
        
        void destroyValue() noexcept;
    };
}

#include "VariantImpl.hpp"