#pragma once

#include "ListInterface.h"
#include "Pair.h"
#include "TypeList.h"

namespace IDragnev::Meta
{
	template <typename List, std::size_t N>
	struct ListRefT : ListRefT<Tail<List>, N - 1> { };

	template <typename List>
	struct ListRefT<List, 0> : HeadT<List> { };

	template <typename List, std::size_t N>
	using ListRef = typename ListRefT<List, N>::type;

	template <typename Lhs, typename Rhs, bool = isEmpty<Rhs>>
	struct ConcatT
	{
		using type = Lhs;
	};

	template <typename Lhs, typename Rhs>
	struct ConcatT<Lhs, Rhs, false> : ConcatT<InsertBack<Lhs, Head<Rhs>>, Tail<Rhs>> { };

	template <typename Lhs, typename Rhs>
	using Concat = typename ConcatT<Lhs, Rhs>::type;

	template <typename List, bool = isEmpty<List>>
	struct ReverseT
	{
	private:
		using ReversedTail = typename ReverseT<Tail<List>>::type;
		using Head = Head<List>;

	public:
		using type = InsertBack<ReversedTail, Head>;
	};

	template<typename List>
	struct ReverseT<List, true>
	{
		using type = List;
	};

	template <typename List>
	using Reverse = typename ReverseT<List>::type;

	template <template <typename, typename> typename F,
		      typename L1,
		      typename L2>
	struct ZipT;

	template <template <typename, typename> typename F,
		      typename... Ts,
		      typename... Fs>
	struct ZipT<F, TypeList<Ts...>, TypeList<Fs...>>
	{
		using type = TypeList<typename F<Ts, Fs>::type...>;
	};

	template <template <typename, typename> typename F,
		      typename L1,
		      typename L2>
	using Zip = typename ZipT<F, L1, L2>::type;

	template <template <typename> typename F,
		      typename List>
	struct MapT;

	template <template <typename> typename F,
		      typename... Vals>
	struct MapT<F, TypeList<Vals...>>
	{
		using type = TypeList<typename F<Vals>::type...>;
	};

	template <template <typename> typename F,
		      typename List>
	using Map = typename MapT<F, List>::type;

	template <typename T>
	struct IdentityT
	{
		using type = T;
	};

	template <typename T>
	struct CloneT
	{
		using type = Pair<T, T>;
	};

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
		      typename List,
	          bool = isEmpty<List>>
	struct FoldLeftT : FoldLeftT<Op, typename Op<Initial, Head<List>>::type, Tail<List>> { };

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
	          typename List>
	struct FoldLeftT<Op, Initial, List, true>
	{
		using type = Initial;
	};

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
		      typename List>
	using FoldLeft = typename FoldLeftT<Op, Initial, List>::type;

	template <typename T, typename F>
	struct LargerT
	{
		using type = std::conditional_t<sizeof(T) >= sizeof(F), T, F>;
	};

	template <typename List>
	struct LargestTypeT : FoldLeftT<LargerT, Head<List>, Tail<List>> { };

	template<>
	struct LargestTypeT<TypeList<>> { };

	template <typename List>
	using LargestType = typename LargestTypeT<List>::type;

	template <typename L>
	using AccReverse = FoldLeft<InsertFrontT, TypeList<>, L>;

	template <typename Lhs, typename Rhs>
	using AccConcat = FoldLeft<InsertBackT, Lhs, Rhs>;

	template <template <typename> typename Predicate,
		      typename List,
	          bool = isEmpty<List>>
	struct FilterT
	{
	private:
		using H = Head<List>;
		using FilteredTail = typename FilterT<Predicate, Tail<List>>::type;
		using Result = std::conditional_t<Predicate<H>::value,
			                              InsertFrontT<FilteredTail, H>,
			                              IdentityT<FilteredTail>>;
	public:
		using type = typename Result::type;
	};

	template <template <typename> typename Predicate,
	          typename List>
	struct FilterT<Predicate, List, true>
	{
		using type = List;
	};

	template <template <typename> typename Predicate,
		      typename List>
	using Filter = typename FilterT<Predicate, List>::type;

	template <unsigned N, typename List>
	struct DropT : DropT<N - 1, Tail<List>> { };

	template <typename List>
	struct DropT<0, List>
	{
		using type = List;
	};

	template <unsigned N, typename List>
	using Drop = typename DropT<N, List>::type;

	template <unsigned N, typename List>
	struct TakeT
	{
		using type = InsertFront<typename TakeT<N - 1, Tail<List>>::type, 
			                     Head<List>>;
	};

	template <typename List>
	struct TakeT<0, List>
	{
		using type = EmptyList<List>;
	};

	template <unsigned N, typename List>
	using Take = typename TakeT<N, List>::type;
}
