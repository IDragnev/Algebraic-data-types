#pragma once

#include "VariantChoice.hpp"
#include "VariantStorage.hpp"
#include "Meta/ListAlgorithms.hpp"
#include "VisitResult.hpp"

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
        template <typename T, typename... AllTypes>
        friend class Detail::VariantChoice;

        template <typename T>
        using VChoice = Detail::VariantChoice<T, Types...>;

    public:
        using VChoice<Types>::VariantChoice...;

        Variant();                                         
        Variant(Variant&& source);                
        Variant(const Variant& source);                    
        ~Variant();
        
        template <typename... SourceTypes>
        Variant(Variant<SourceTypes...>&& source);

        template <typename... SourceTypes>
        Variant(const Variant<SourceTypes...>& source);
        
        using VChoice<Types>::operator=...;

        Variant& operator=(Variant&& source);
        Variant& operator=(const Variant& source);

        template <typename... SourceTypes>
        Variant& operator=(Variant<SourceTypes...>&& source);

        template <typename... SourceTypes>
        Variant& operator=(const Variant<SourceTypes...>& source);

        template <typename T>
        bool is() const noexcept;          
        
        template <typename T> 
        T& get() &;

        template <typename T> 
        T&& get() &&;

        template <typename T> 
        const T& get() const&;      

        bool isEmpty() const noexcept;
 
    private:
        static constexpr unsigned char NO_VALUE_DISCRIMINATOR = 0;
        
        template <typename VariantT>
        void copyFromIfNotEmpty(VariantT&& source);
        template <typename VariantT>
        void copyFrom(VariantT&& source);
        template <typename VariantT>
        void assignFrom(VariantT&& source);
        void destroyValue() noexcept;
    };

    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, Types&...> 
    visit(Variant<Types...>& variant, Visitor&& v);
        
    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, const Types&...>
    visit(const Variant<Types...>& variant, Visitor&& v);
        
    template <typename R = Detail::DeduceResultType,
              typename... Types,
              typename Visitor
    > Detail::VisitResult<R, Visitor, Types&&...>
    visit(Variant<Types...>&& variant, Visitor&& v);
}

#include "VariantImpl.hpp"
