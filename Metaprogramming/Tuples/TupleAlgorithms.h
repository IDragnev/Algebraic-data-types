#pragma once

#include "Lists\ListInterface.h"
#include "Lists\List algorithms.h"
#include "Tuple.h"

namespace IDragnev::Meta
{
	template <typename H, typename... Tail>
	struct HeadT<Tuple<H, Tail...>>
	{
		using type = H;
	};

	template <typename T, typename... Ts>
	struct TailT<Tuple<T, Ts...>>
	{
		using type = Tuple<Ts...>;
	};

	template <typename... Elems, typename... Ts>
	struct InsertFrontT<Tuple<Elems...>, Ts...>
	{
		using type = Tuple<Ts..., Elems...>;
	};

	template <>
	struct IsEmpty<Tuple<>> : std::true_type { };

	template <typename... Elems, typename... Ts>
	struct InsertBackT<Tuple<Elems...>, Ts...>
	{
		using type = Tuple<Elems..., Ts...>;
	};

	template <typename... Elems>
	struct EmptyListT<Tuple<Elems...>>
	{
		using type = Tuple<>;
	};

	template <typename T, typename... Elements>
	auto insertFront(const Tuple<Elements...>& tuple, const T& value)
	{
		using Result = Tuple<T, Elements...>;
		return Result(value, tuple);
	}

	template <typename T>
	auto insertBack(const Tuple<>& tuple, const T& value)
	{
		return Tuple<T>(value);
	}

	template <typename T, typename Head, typename... Tail>
	auto insertBack(const Tuple<Head, Tail...>& tuple, const T& value)
	{
		using Result = Tuple<Head, Tail..., T>;
		return Result(tuple.getHead(), 
			          insertBack(tuple.getTail(), value));
	}

	void dropHead(const Tuple<>&) = delete;

	template <typename H, typename... Elements>
	inline auto dropHead(const Tuple<H, Elements...>& tuple)
	{
		return tuple.getTail();
	}

	void dropTail(const Tuple<>&) = delete;

	template <typename H, typename... Elements>
	inline auto dropTail(const Tuple<H, Elements...>& tuple)
	{
		return Tuple<H>(tuple.getHead());
	}

	namespace Detail
	{
		template <typename... Elements, unsigned... Indices>
		inline auto select(const Tuple<Elements...>& tuple, ValueList<unsigned, Indices...>)
		{
			return makeTuple(get<Indices>(tuple)...);
		}
	}

	template <unsigned... Indices, typename... Elements>
	inline auto select(const Tuple<Elements...>& tuple)
	{
		return Detail::select(tuple, ValueList<unsigned, Indices...>{});
	}

	template <typename... Elements>
	inline auto reverse(const Tuple<Elements...>& tuple)
	{
		using Indices = Reverse<MakeIndexList<sizeof...(Elements)>>;
		return Detail::select(tuple, Indices{});
	}

	template <unsigned Index, unsigned N, typename... Elements>
	inline auto replicate(const Tuple<Elements...>& t)
	{
		return Detail::select(t, ReplicateValue<Index, N>{});
	}

	template <unsigned N, typename... Elements>
	inline auto take(const Tuple<Elements...>& t)
	{
		using Indices = Take<N, MakeIndexList<sizeof...(Elements)>>;
		return Detail::select(t, Indices{});
	}

	template <unsigned N, typename... Elements>
	inline auto drop(const Tuple<Elements...>& t)
	{
		using Indices = Drop<N, MakeIndexList<sizeof...(Elements)>>;
		return Detail::select(t, Indices{});
	}

	template <template <typename U, typename V> typename CompareFn,
		      typename... Elements>
	inline auto sortByType(const Tuple<Elements...>& t)
	{
		using Indices = InsertionSort<MakeIndexList<sizeof...(Elements)>,
			                          MakeIndexedCompareT<Tuple<Elements...>, CompareFn>::template Compare>;
		return Detail::select(t, Indices{});
	}
}