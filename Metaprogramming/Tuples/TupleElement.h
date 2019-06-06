#pragma once

#include <type_traits>

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
		TupleElement(U&& value) : value(std::forward<U>(value)) { }

		T& get() noexcept { return value; }
		const T& get() const noexcept { return value; }

	private:
		T value;
	};

	template <unsigned Height, typename T>
	class TupleElement<Height, T, true> : private T
	{
	public:
		TupleElement() = default;
		template<typename U>
		TupleElement(U&& value) : T(std::forward<U>(value)) { }

		T& get() noexcept { return *this; }
		const T& get() const noexcept { return *this; }
	};

	template <unsigned Height, typename T>
	T& extractValue(TupleElement<Height, T>& e) noexcept
	{
		const auto& element = e;
		return const_cast<T&>(extractValue(element));
	}

	template <unsigned Height, typename T>
	const T& extractValue(const TupleElement<Height, T>& e) noexcept
	{
		return e.get();
	}
}