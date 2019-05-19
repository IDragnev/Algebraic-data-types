#include <type_traits>

namespace IDragnev::Meta
{
	template <typename... Elements>
	struct List;

	template <typename List>
	struct HeadT;

	template <typename H, typename... Tail>
	struct HeadT<List<H, Tail...>>
	{
		using type = H;
	};

	template <typename List>
	using Head = typename HeadT<List>::type;

	template <typename List>
	struct TailT;

	template <typename T, typename... Ts>
	struct TailT<List<T, Ts...>>
	{
		using type = List<Ts...>;
	};

	template <typename List>
	using Tail = typename TailT<List>::type;

	template <typename List, typename T>
	struct InsertFrontT;

	template <typename... Elems, typename T>
	struct InsertFrontT<List<Elems...>, T>
	{
		using type = List<T, Elems...>;
	};

	template <typename List, typename T>
	using InsertFront = typename InsertFrontT<List, T>::type;

	template <typename List, int N>
	struct ListRefT : ListRefT<Tail<List>, N - 1> { };

	template <typename List>
	struct ListRefT<List, 0> : HeadT<List> { };

	template <typename List, int N>
	using ListRef = typename ListRefT<List, N>::type;

	static_assert(std::is_same_v<ListRef<List<int, double>, 1>,
	 	                         double>);

	template <typename List>
	struct IsEmpty : std::false_type { };

	template <>
	struct IsEmpty<List<>> : std::true_type { };

	template <typename List>
	inline constexpr bool isEmpty = IsEmpty<List>::value;

	template <typename Lhs, typename Rhs>
	struct ConcatT;

	template <typename... Ts, typename... Fs>
	struct ConcatT<List<Ts...>, List<Fs...>>
	{
		using type = List<Ts..., Fs...>;
	};

	template <typename Lhs, typename Rhs>
	using Concat = typename ConcatT<Lhs, Rhs>::type;

	template <typename L, typename... Ts>
	struct InsertBackT : ConcatT<L, List<Ts...>> { };

	template <typename List, typename... Ts>
	using InsertBack = typename InsertBackT<List, Ts...>::type;

	template <typename List>
	struct ReverseT
	{
	private:
		using ReversedTail = typename ReverseT<Tail<List>>::type;
		using Head = Head<List>;

	public:
		using type = InsertBack<ReversedTail, Head>;
	};

	template<>
	struct ReverseT<List<>>
	{
		using type = List<>;
	};

	template <typename List>
	using Reverse = typename ReverseT<List>::type;

	static_assert(std::is_same_v<Reverse<List<int, double>>,
		                         List<double, int>>);

	template <typename T, typename F>
	struct Pair;

	template <typename Pair>
	struct FirstT;

	template <typename T, typename F>
	struct FirstT<Pair<T, F>>
	{
		using type = T;
	};

	template <typename Pair>
	using First = typename FirstT<Pair>::type;

	template <typename Pair>
	struct SecondT;

	template <typename T, typename F>
	struct SecondT<Pair<T, F>>
	{
		using type = F;
	};

	template <typename Pair>
	using Second = typename SecondT<Pair>::type;

	template <template <typename, typename> typename F,
		      typename L1,
		      typename L2>
	struct ZipT;

	template <template <typename, typename> typename F,
		      typename... Ts,
		      typename... Fs>
	struct ZipT<F, List<Ts...>, List<Fs...>>
	{
		using type = List<typename F<Ts, Fs>::type...>;
	};

	template <template <typename, typename> typename F,
		      typename L1,
		      typename L2>
	using Zip = typename ZipT<F, L1, L2>::type;

	template <typename T, typename F>
	struct MakePairT
	{
		using type = Pair<T, F>;
	};

	template <typename T, typename F>
	using MakePair = typename MakePairT<T, F>::type;

	static_assert(std::is_same_v<Zip<MakePairT, List<int, char, bool>, List<float, double, long>>,
		                         List<Pair<int, float>, Pair<char, double>, Pair<bool, long>>>);

	static_assert(std::is_same_v<Zip<std::common_type, List<int, char, bool>, List<unsigned, char16_t, bool>>,
		                         List<unsigned, int, bool>>);

	template <typename... Types>
	struct ArgList
	{
		template <Types...>
		struct Values;
	};

	ArgList<int, char, unsigned>::Values<1, '2', 3>;

	template <template <typename> typename F,
		      typename List>
	struct MapT;

	template <template <typename> typename F,
		      typename... Vals>
	struct MapT<F, List<Vals...>>
	{
		using type = List<typename F<Vals>::type...>;
	};

	template <template <typename> typename F,
		      typename List>
	using Map = typename MapT<F, List>::type;

	static_assert(std::is_same_v<Map<std::add_lvalue_reference, List<int, char, double>>,
		                         List<int&, char&, double&>>);

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

	static_assert(std::is_same_v<Map<CloneT, List<int, double>>,
		                         List<Pair<int, int>, Pair<double, double>>>);
	static_assert(std::is_same_v<Map<HeadT, List<List<int, double>, List<char, float>, List<unsigned, short>>>,
		                         List<int, char, unsigned>>);
	static_assert(std::is_same_v<Map<IdentityT, List<int, float, double>>,
		                         List<int, float, double>>);

	template <template <typename Res, typename Current> typename Op,
		      typename Initial,
		      typename List>
	struct FoldLeftT : FoldLeftT<Op, typename Op<Initial, Head<List>>::type, Tail<List>> { };

	template <template <typename Res, typename Current> typename Op,
		      typename Initial>
	struct FoldLeftT<Op, Initial, List<>>
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

	template <>
	struct LargestTypeT<List<>> { };

	template <typename List>
	using LargestType = typename LargestTypeT<List>::type;

	static_assert(std::is_same_v<LargestType<List<char, unsigned, long double>>, long double>);

	template <typename L>
	using AccReverse = FoldLeft<InsertFrontT, List<>, L>;

	static_assert(std::is_same_v<AccReverse<List<int, double, char>>,
		                         List<char, double, int>>);

	template <typename Lhs, typename Rhs>
	using AccConcat = FoldLeft<InsertBackT, Lhs, Rhs>;

	static_assert(std::is_same_v<AccConcat<List<int, double>, List<float, char>>,
		                         List<int, double, float, char>>);

	template <template <typename> typename Predicate,
		typename List>
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

	template <template <typename> typename Predicate>
	struct FilterT<Predicate, List<>>
	{
		using type = List<>;
	};

	template <template <typename> typename Predicate,
		      typename List>
	using Filter = typename FilterT<Predicate, List>::type;

	static_assert(std::is_same_v<Filter<std::is_integral, List<int, float, double, unsigned, bool>>,
		                         List<int, unsigned, bool>>);
	static_assert(std::is_same_v<Filter<std::is_reference, List<int, const int, const char, std::size_t, double>>,
		                         List<>>);
}
