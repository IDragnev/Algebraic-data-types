#pragma once

#include <type_traits>
#include <utility>

namespace IDragnev::Meta::Detail
{
	template <typename T>
	inline constexpr auto canBeInherited = std::is_class_v<T> && !std::is_final_v<T>;

	template <unsigned Height,
		      typename T,
		      bool = canBeInherited<T>
	> class TupleElement;

	template <unsigned Height, typename T>
	class TupleElement<Height, T, false>
	{
	public:
		TupleElement() = default;
		template<typename U>
		constexpr TupleElement(U&& value) : value(std::forward<U>(value)) { }

		constexpr T&& get() && noexcept { return std::move(value); }
		constexpr const T&& get() const && noexcept { return std::move(value); }
		constexpr T& get() & noexcept { return value; }
		constexpr const T& get() const & noexcept { return value; }

	private:
		T value;
	};

	template <unsigned Height, typename T>
	class TupleElement<Height, T, true> : private T
	{
	public:
		TupleElement() = default;
		template<typename U>
		constexpr TupleElement(U&& value) : T(std::forward<U>(value)) { }

		constexpr T&& get() && noexcept { return std::move(*this); }
		constexpr const T&& get() const && noexcept { return std::move(*this); }
		constexpr T& get() & noexcept { return *this; }
		constexpr const T& get() const & noexcept { return *this; }
	};

	template <unsigned Height, typename T>
	inline constexpr 
	const T& extractValue(const TupleElement<Height, T>& e) noexcept
	{
		return e.get();
	}

	template <unsigned Height, typename T>
	inline constexpr
	T& extractValue(TupleElement<Height, T>& e) noexcept
	{
		return const_cast<T&>(extractValue(std::as_const(e)));
	}

	template <unsigned Height, typename T>
	inline constexpr 
	const T&& extractValue(const TupleElement<Height, T>&& e) noexcept
	{
		return std::move(extractValue(e));
	}

	template <unsigned Height, typename T>
	inline constexpr
	T&& extractValue(TupleElement<Height, T>&& e) noexcept
	{
		return std::move(e).get();
	}
}