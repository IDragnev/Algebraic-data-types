#pragma once

#include "VariantChoice.h"
#include "VariantStorage.h"
#include "Meta\List algorithms.h"

#include <stdexcept>

namespace IDragnev
{
    namespace Detail
    {
        template <typename... Args>
        struct VisitResult;
    }

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
        Variant(Variant const& source);                    
        Variant(Variant&& source);                   
        ~Variant();
        
        template <typename... SourceTypes>
        Variant(const Variant<SourceTypes...>& source);
        
        template<typename... SourceTypes>
        Variant(Variant<SourceTypes...>&& source);

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
        const T& get() const&;      
        
        template <typename T> 
        T&& get() &&;

        template <typename R = ComputedResultType, typename Visitor>
        Detail::VisitResult<R, Visitor, Types&...> 
        visit(Visitor&& v) &;
        
        template <typename R = ComputedResultType, typename Visitor>
        Detail::VisitResult<R, Visitor, const Types&...>
        visit(Visitor&& v) const&;
        
        template<typename R = ComputedResultType, typename Visitor>
        Detail::VisitResult<R, Visitor, Types&&...>
        visit(Visitor&& v) &&;

        bool isEmpty() const noexcept;

    private:
        static constexpr unsigned NO_VALUE_DISCRIMINATOR = 0;
        
        void destroyValue() noexcept;
    };
}

#include "VariantImpl.hpp"