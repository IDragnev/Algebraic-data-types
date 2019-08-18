#pragma once

#include <new>
#include "Meta/ListAlgorithms.h"

namespace IDragnev::Detail
{
    template <typename... Types>
    class VariantStorage
    {
    private:
        using LargestT = Meta::LargestType<Meta::TypeList<Types...>>;
        
    public:
        unsigned char getDiscriminator() const noexcept;
        void setDiscriminator(unsigned char d) noexcept;
        
        void* getRawBuffer() noexcept;
        const void* getRawBuffer() const noexcept;

        template <typename T>
        T* getBufferAs() noexcept;

        template <typename T>
        const T* getBufferAs() const noexcept;

    private:
        alignas(Types...) unsigned char buffer[sizeof(LargestT)];
        unsigned char discriminator = 0;
    };

    template <typename... Types>
    inline
    unsigned char VariantStorage<Types...>::getDiscriminator() const noexcept 
    {
        return discriminator;
    }

    template <typename... Types>
    inline 
    void VariantStorage<Types...>::setDiscriminator(unsigned char d) noexcept
    {
        discriminator = d;
    }   
    
    template <typename... Types>
    inline
    void* VariantStorage<Types...>::getRawBuffer() noexcept
    {
        return buffer;
    }

    template <typename... Types>
    inline
    const void* VariantStorage<Types...>::getRawBuffer() const noexcept 
    {
        return buffer;
    }

    template <typename... Types>
    template <typename T>
    inline T* VariantStorage<Types...>::getBufferAs() noexcept
    {
        return std::launder(reinterpret_cast<T*>(buffer));
    }

    template <typename... Types>
    template <typename T>
    inline const T* VariantStorage<Types...>::getBufferAs() const noexcept
    {
        return std::launder(reinterpret_cast<const T*>(buffer));
    }
}