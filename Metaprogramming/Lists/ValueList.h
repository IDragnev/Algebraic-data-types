#pragma once

#include "ListInterface.h"

namespace IDragnev::Meta
{
	template <typename T, T Value>
	struct CTValue
	{
		static constexpr T value = Value;
	};

	template <typename T, T... values>
	struct ValueList { };

	template <typename T, T... values>
	struct IsEmpty<ValueList<T, values...>> : std::bool_constant<sizeof...(values) == 0> { };

	template <typename T, T head, T... tail>
	struct HeadT<ValueList<T, head, tail...>>
	{
		using type = CTValue<T, head>;
		static constexpr T value = head;
	};

	template <typename T>
	struct HeadT<ValueList<T>> { };

	template <typename T, T head, T... tail>
	struct TailT<ValueList<T, head, tail...>>
	{
		using type = ValueList<T, tail...>;
	};

	template <typename T>
	struct TailT<ValueList<T>> { };

	template <typename T, T... elems, T value>
	struct InsertFrontT<ValueList<T, elems...>, CTValue<T, value>>
	{
		using type = ValueList<T, value, elems...>;
	};

	template <typename T, T... elems, T value>
	struct InsertBackT<ValueList<T, elems...>, CTValue<T, value>>
	{
		using type = ValueList<T, elems..., value>;
	};

	template <typename T, T... values>
	struct EmptyListT<ValueList<T, values...>>
	{
		using type = ValueList<T>;
	};
}