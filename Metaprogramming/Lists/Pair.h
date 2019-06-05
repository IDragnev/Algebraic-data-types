#pragma once

namespace IDragnev::Meta
{
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

	template <typename T, typename F>
	struct MakePairT
	{
		using type = Pair<T, F>;
	};

	template <typename T, typename F>
	using MakePair = typename MakePairT<T, F>::type;
}