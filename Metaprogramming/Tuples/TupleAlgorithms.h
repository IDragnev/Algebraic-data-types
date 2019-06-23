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

	template <>
	struct HeadT<Tuple<>> { };

	template <typename T, typename... Ts>
	struct TailT<Tuple<T, Ts...>>
	{
		using type = Tuple<Ts...>;
	};

	template <>
	struct TailT<Tuple<>> { };

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
	inline constexpr
	auto insertFront(const Tuple<Elements...>& tuple, const T& value)
	{
		using Result = Tuple<T, Elements...>;
		return Result(value, tuple);
	}

	template <typename T>
	inline constexpr 
	auto insertBack(const Tuple<>& tuple, const T& value)
	{
		return Tuple<T>(value);
	}

	template <typename T, typename Head, typename... Tail>
	constexpr auto insertBack(const Tuple<Head, Tail...>& tuple, const T& value)
	{
		using Result = Tuple<Head, Tail..., T>;
		return Result(tuple.getHead(), 
			          insertBack(tuple.getTail(), value));
	}

	template <typename H, typename... Elements>
	inline constexpr 
	auto dropHead(const Tuple<H, Elements...>& tuple)
	{
		return tuple.getTail();
	}

	template <typename H, typename... Elements>
	inline constexpr
	auto dropTail(const Tuple<H, Elements...>& tuple)
	{
		return Tuple<H>(tuple.getHead());
	}

	namespace Detail
	{
		template <typename TupleT, unsigned... Indices>
		inline constexpr
		auto select(TupleT&& tuple, ValueList<unsigned, Indices...>)
		{
			return makeTuple(get<Indices>(std::forward<TupleT>(tuple))...);
		}

		template <typename T>
		using EnableIfTuple = std::enable_if_t<isTuple<std::decay_t<T>>>;
	}

	template <unsigned... Indices, 
		      typename TupleT,
		      typename = Detail::EnableIfTuple<TupleT>>
	inline constexpr
	auto select(TupleT&& tuple)
	{
		using IndicesT = ValueList<unsigned, Indices...>;
		return Detail::select(std::forward<TupleT>(tuple), IndicesT{});
	}

	template <typename TupleT,
	          unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>>
	inline constexpr
	auto reverse(TupleT&& tuple)
	{
		using Indices = Reverse<MakeIndexList<Size>>;
		return Detail::select(std::forward<TupleT>(tuple), Indices{});
	}

	template <unsigned Index, unsigned N, typename... Elements>
	inline constexpr
	auto replicate(const Tuple<Elements...>& t)
	{
		using Indices = ReplicateValue<Index, N>;
		return Detail::select(t, Indices{});
	}

	template <unsigned N, 
	          typename TupleT,
	          unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>
	> inline constexpr
	auto take(TupleT&& t)
	{
		using Indices = Take<N, MakeIndexList<Size>>;
		return Detail::select(std::forward<TupleT>(t), Indices{});
	}

	template <unsigned N,
	          typename TupleT,
	          unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>
	> inline constexpr 
	auto drop(TupleT&& t)
	{
		using Indices = Drop<N, MakeIndexList<Size>>;
		return Detail::select(std::forward<TupleT>(t), Indices{});
	}

	template <template <typename U, typename V> typename CompareFn,
		      typename... Elements>
	inline constexpr
	auto sortByType(const Tuple<Elements...>& t)
	{
		using Indices = InsertionSort<MakeIndexList<sizeof...(Elements)>,
			                          MakeIndexedCompareT<Tuple<Elements...>, CompareFn>::template Compare>;
		return Detail::select(t, Indices{});
	}

	namespace Detail
	{
		template <typename Callable, typename... Args>
		constexpr void forEachArg(Callable f, Args&&... args)
		{
			(f(std::forward<Args>(args)), ...);
		}

		template <typename Callable, 
			      typename TupleT,
			      unsigned... Indices
		> inline constexpr
		void forEach(TupleT&& tuple, Callable f, ValueList<unsigned, Indices...>)
		{
			forEachArg(f, get<Indices>(std::forward<TupleT>(tuple))...);
		}
	}

	template <typename Callable, 
		      typename TupleT,
	          unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>
	> inline constexpr
	void forEach(TupleT&& tuple, Callable f)
	{
		using Indices = MakeIndexList<Size>;
		Detail::forEach(std::forward<TupleT>(tuple), f, Indices{});
	}

	namespace Detail
	{
		template <typename Callable, 
			      typename TupleT, 
			      unsigned... Indices
		> inline constexpr 
		decltype(auto) apply(Callable f, TupleT&& tuple, ValueList<unsigned, Indices...>)
		{
			return f(get<Indices>(std::forward<TupleT>(tuple))...);
		}
	}

	template <typename Callable,
		      typename TupleT,
		      unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>
	> inline constexpr
	decltype(auto) apply(Callable f, TupleT&& tuple)
	{
		using Indices = MakeIndexList<Size>;
		return Detail::apply(f, std::forward<TupleT>(tuple), Indices{});
	}

	namespace Detail
	{
		template <typename UTuple,
			      typename VTuple,
				  unsigned... UIndices,
			      unsigned... VIndices
		> constexpr auto concatenate(UTuple&& u, VTuple&& v,
			                         ValueList<unsigned, UIndices...>,
			                         ValueList<unsigned, VIndices...>)
		{
			return makeTuple(get<UIndices>(std::forward<UTuple>(u))...,
				             get<VIndices>(std::forward<VTuple>(v))...);
		}

		template <typename... Types>
		inline constexpr bool areAllTuples = allOf<IsTuple, TypeList<std::decay_t<Types>...>>;
	}

	template <typename UTuple,
		      typename VTuple,
		      unsigned USize = Detail::tupleSize<std::decay_t<UTuple>>,
		      unsigned VSize = Detail::tupleSize<std::decay_t<VTuple>>
	> constexpr auto concatenate(UTuple&& u, VTuple&& v)
	{
		using UIndices = MakeIndexList<USize>;
		using VIndices = MakeIndexList<VSize>;

		return Detail::concatenate(std::forward<UTuple>(u), 
								   std::forward<VTuple>(v),
			                       UIndices{},
			                       VIndices{});
	}

	template <typename UTuple,
		      typename VTuple,
		      typename... Tuples,
		      typename = std::enable_if_t<Detail::areAllTuples<UTuple, VTuple, Tuples...>>
	> constexpr auto concatenate(UTuple&& u, VTuple&& v, Tuples&&... rest)
	{
		return concatenate(concatenate(std::forward<UTuple>(u), 
			                           std::forward<VTuple>(v)),
			               std::forward<Tuples>(rest)...);
	}
}