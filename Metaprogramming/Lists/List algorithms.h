#pragma once

#include "TypeFunctionsAndPredicates.h"
#include "TypeList.h"
#include "ValueList.h"
#include "Pair.h"

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
	struct ReverseT;

	template <typename List>
	using Reverse = typename ReverseT<List>::type;

	template <typename List>
	struct ReverseT<List, false> : InsertBackT<Reverse<Tail<List>>, Head<List>> { };

	template<typename List>
	struct ReverseT<List, true>
	{
		using type = List;
	};

	template <template <typename> typename F,
		      typename List
	> struct MapT;

	template <template <typename> typename F,
		      typename... Ts
	> struct MapT<F, TypeList<Ts...>>
	{
		using type = TypeList<typename F<Ts>::type...>;
	};

	template <template <typename> typename F,
		      typename List
	> using Map = typename MapT<F, List>::type;

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
		      typename List,
	          bool = isEmpty<List>
	> struct FoldLeftT : FoldLeftT<Op, typename Op<Initial, Head<List>>::type, Tail<List>> { };

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
	          typename List>
	struct FoldLeftT<Op, Initial, List, true>
	{
		using type = Initial;
	};

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
		      typename List
	> using FoldLeft = typename FoldLeftT<Op, Initial, List>::type;

	template <typename List>
	struct LargestTypeT : FoldLeftT<LargerT, Head<List>, Tail<List>> { };

	template<>
	struct LargestTypeT<TypeList<>> { };

	template <typename List>
	using LargestType = typename LargestTypeT<List>::type;

	template <typename List>
	using FReverse = FoldLeft<InsertFrontT, EmptyList<List>, List>;

	template <typename Lhs, typename Rhs>
	using FConcat = FoldLeft<InsertBackT, Lhs, Rhs>;

	template <template <typename> typename Predicate,
		      typename List,
		      bool = isEmpty<List>
	> struct FilterT;

	template <template <typename> typename Predicate,
		      typename List
	> using Filter = typename FilterT<Predicate, List>::type;

	template <template <typename> typename Predicate,
		      typename List
	> struct FilterT<Predicate, List, false>
	{
	private:
		using H = Head<List>;
		using FilteredTail = Filter<Predicate, Tail<List>>;
		using Result = std::conditional_t<Predicate<H>::value,
			                              InsertFrontT<FilteredTail, H>,
			                              IdentityT<FilteredTail>>;
	public:
		using type = typename Result::type;
	};

	template <template <typename> typename Predicate,
	          typename List
	> struct FilterT<Predicate, List, true>
	{
		using type = List;
	};

	template <template <typename> typename Predicate>
	struct MakeConditionalInsertBack
	{
		template <typename List, typename T>
		struct Lambda
		{
		private:
			using Result = std::conditional_t<Predicate<T>::value,
											  InsertBackT<List, T>,
										      IdentityT<List>>;
		public:
			using type = typename Result::type;
		};
	};

	template <template <typename> typename Predicate,
		      typename List
	> using FFilter = FoldLeft<MakeConditionalInsertBack<Predicate>::template Lambda, EmptyList<List>, List>;
	
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
	struct TakeT;

	template <unsigned N, typename List>
	using Take = typename TakeT<N, List>::type;

	template <unsigned N, typename List>
	struct TakeT : 
		InsertFrontT<Take<N - 1, Tail<List>>, 
			         Head<List>>
	{ };

	template <typename List>
	struct TakeT<0, List> : EmptyListT<List> { };

	template <unsigned I, typename List>
	struct SplitAtT : MakePairT<Take<I, List>, Drop<I, List>> { };

	template <unsigned I, typename List>
	using SplitAt = typename SplitAtT<I, List>::type;

	template <unsigned Size, typename Result = ValueList<unsigned>>
	struct MakeIndexListT : 
		MakeIndexListT<Size - 1, 
		               InsertFront<Result, CTValue<unsigned, Size - 1>>> 
	{ };

	template <typename Result>
	struct MakeIndexListT<0, Result>
	{
		using type = Result;
	};

	template <unsigned N>
	using MakeIndexList = typename MakeIndexListT<N>::type;

	template <auto Value,
		      unsigned Count,
		      typename Result = ValueList<decltype(Value)>
	> struct ReplicateValueT : 
		ReplicateValueT<Value, 
					    Count - 1, 
			            InsertFront<Result, CTValue<decltype(Value), Value>>> 
	{ };

	template <auto Value, typename Result>
	struct ReplicateValueT<Value, 0, Result>
	{
		using type = Result;
	};

	template <auto V, unsigned Count>
	using ReplicateValue = typename ReplicateValueT<V, Count>::type;

	template <typename T,
		      typename List,
		      template <typename U, typename V> typename Compare,
		      bool = isEmpty<List>
	> struct InsertInSortedT;

	template <typename T,
		      typename List,
		      template <typename U, typename V> typename Compare
	> using InsertInSorted = typename InsertInSortedT<T, List, Compare>::type;

	template <typename List,
		      template <typename U, typename V> typename Compare,
		      bool = isEmpty<List>
	> struct InsertionSortT;

	template <typename List,
		      template <typename U, typename V> typename Compare
	> using InsertionSort = typename InsertionSortT<List, Compare>::type;

	template <typename List,
		      template <typename U, typename V> typename Compare
    > struct InsertionSortT<List, Compare, true>
	{
		using type = List;
	};

	template <typename List,
		      template <typename U, typename V> typename Compare
	> struct InsertionSortT<List, Compare, false> : 
		InsertInSortedT<Head<List>, InsertionSort<Tail<List>, Compare>, Compare> { };

	template <typename T,
		      typename List,
		      template <typename U, typename V> typename Compare
	> struct InsertInSortedT<T, List, Compare, true> : InsertFrontT<List, T> { };

	template <typename T,
		      typename List,
		      template <typename U, typename V> typename Compare
	> struct InsertInSortedT<T, List, Compare, false>
	{
	private:
		using NewTailT = std::conditional_t<Compare<T, Head<List>>::value,
			                                IdentityT<List>,
			                                InsertInSortedT<T, Tail<List>, Compare>>;
		using NewTail = typename NewTailT::type;
		using NewHead = std::conditional_t<Compare<T, Head<List>>::value,
			                               T,
			                               Head<List>>;
	public:
		using type = InsertFront<NewTail, NewHead>;
	};

	template <typename List,
		      template <typename U, typename V> typename CompareFn
	> struct MakeIndexedCompareT
	{
		template <typename T, typename F> struct Compare;

		template <unsigned I, unsigned J>
		struct Compare<CTValue<unsigned, I>, CTValue<unsigned, J>> : 
			CompareFn<ListRef<List, I>, ListRef<List, J>> { };
	};

	template <template <typename T> typename Predicate,
		      typename List,
		      bool = isEmpty<List>
	> struct AllOfT;

	template <template <typename T> typename Predicate,
		      typename List>
	inline constexpr bool allOf = AllOfT<Predicate, List>::value;

	template <template <typename T> typename Predicate,
		      typename List>
	struct AllOfT<Predicate, List, true> : std::true_type { };

	template <template <typename T> typename Predicate,
		      typename List
	> struct AllOfT<Predicate, List, false> : 
		std::bool_constant<Predicate<Head<List>>::value && 
						   allOf<Predicate, Tail<List>>> 
	{ };

	template <template <typename T> typename Predicate,
		      typename List
	> struct NoneOfT : AllOfT<Inverse<Predicate>::template Result, List> { };

	template <template <typename T> typename Predicate,
		      typename List>
	inline constexpr bool noneOf = NoneOfT<Predicate, List>::value;

	template <template <typename T> typename Predicate,
		      typename List
	> struct AnyOfT : std::bool_constant<!allOf<Inverse<Predicate>::template Result, List>> { };

	template <template <typename T> typename Predicate,
		      typename List>
	inline constexpr bool anyOf = AnyOfT<Predicate, List>::value;

	template <typename T,
		      typename List,
		      bool = isEmpty<List>
	> struct IsMemberT;

	template <typename T, typename List>
	inline constexpr bool isMember = IsMemberT<T, List>::value;

	template <typename T, typename List> 
	struct IsMemberT<T, List, true> : std::false_type { };

	template <typename T, typename List>
	struct IsMemberT<T, List, false> 
	{
	private:
		using Result = std::conditional_t<std::is_same_v<T, Head<List>>,
										  std::true_type,
										  IsMemberT<T, Tail<List>>>;
	public:
		static inline constexpr bool value = Result::value;
	};

	template <typename List, bool = isEmpty<List>>
	struct MakeSetT;

	template <typename List>
	using MakeSet = typename MakeSetT<List>::type;

	template <typename List>
	struct MakeSetT<List, true>
	{
		using type = List;
	};

	template <typename List>
	struct MakeSetT<List, false>
	{
	private:
		using TailSet = MakeSet<Tail<List>>;	
		using Result = std::conditional_t<isMember<Head<List>, Tail<List>>,
										  IdentityT<TailSet>,
						                  InsertFrontT<TailSet, Head<List>>>;
	public:
		using type = typename Result::type;
	 };

	namespace Detail
	{
		template <template <typename... Args> typename F,
			      typename PackedLists,
			      bool = anyOf<IsEmpty, PackedLists>
		> struct ZipImplT;

		template <template <typename... Args> typename F,
			      typename PackedLists
		> using ZipImpl = typename ZipImplT<F, PackedLists>::type;

		template <template <typename... Args> typename F,
			      typename HeadList,
				  typename... Lists
		> struct ZipImplT<F, TypeList<HeadList, Lists...>, true> : EmptyListT<HeadList> { };

		template <template <typename... Args> typename F,
			      typename... Lists
		> struct ZipImplT<F, TypeList<Lists...>, false> :
			InsertFrontT<ZipImpl<F, TypeList<Tail<Lists>...>>, 
			             typename F<Head<Lists>...>::type> 
		{ };
	}
	
	template <template <typename... Args> typename F,
	          typename HeadList, 
			  typename... Lists
	> struct ZipT : Detail::ZipImplT<F, TypeList<HeadList, Lists...>> { };

	template <template <typename... Args> typename F,
		      typename... Lists
	> using Zip = typename ZipT<F, Lists...>::type;
}
