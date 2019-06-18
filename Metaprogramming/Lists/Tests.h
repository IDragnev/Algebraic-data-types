#pragma once

#include "List algorithms.h"
#include "TypeList.h"
#include "ValueList.h"

namespace IDragnev::Meta
{
	static_assert(isEmpty<TypeList<>>);

	static_assert(!isEmpty<TypeList<int>>);

	static_assert(std::is_same_v<EmptyList<TypeList<int>>, TypeList<>>);

	static_assert(std::is_same_v<Head<TypeList<int, float>>, int>);

	static_assert(std::is_same_v<Tail<TypeList<int, float, char>>, TypeList<float, char>>);

	static_assert(std::is_same_v<Tail<TypeList<int>>, TypeList<>>);

	static_assert(std::is_same_v<InsertFront<TypeList<>, double>,
		                         TypeList<double>>);

	static_assert(std::is_same_v<InsertFront<TypeList<int>, double>,
		                         TypeList<double, int>>);

	static_assert(std::is_same_v<InsertBack<TypeList<>, double>,
		                         TypeList<double>>);

	static_assert(std::is_same_v<InsertBack<TypeList<int>, double>,
		                         TypeList<int, double>>);

	static_assert(isEmpty<ValueList<int>>);

	static_assert(!isEmpty<ValueList<int, 1>>);

	static_assert(std::is_same_v<EmptyList<ValueList<int, 1, 2>>, ValueList<int>>);

	static_assert(std::is_same_v<Head<ValueList<int, 1, 2>>, CTValue<int, 1>>);

	static_assert(std::is_same_v<Tail<ValueList<int, 1, 2, 3>>, ValueList<int, 2, 3>>);

	static_assert(std::is_same_v<Tail<ValueList<int, 1>>, ValueList<int>>);

	static_assert(std::is_same_v<InsertFront<ValueList<int>, CTValue<int, 1>>,
		                         ValueList<int, 1>>);

	static_assert(std::is_same_v<InsertFront<ValueList<int, 1, 2>, CTValue<int, 0>>,
		                         ValueList<int, 0, 1, 2>>);

	static_assert(std::is_same_v<InsertBack<ValueList<int>, CTValue<int, 1>>,
		                         ValueList<int, 1>>);

	static_assert(std::is_same_v<InsertBack<ValueList<int, 1, 2>, CTValue<int, 3>>,
		                         ValueList<int, 1, 2, 3>>);

	static_assert(std::is_same_v<ListRef<TypeList<int, double>, 1>,
		                         double>);
	
	static_assert(std::is_same_v<Reverse<TypeList<int, double>>,
		                         TypeList<double, int>>);

	static_assert(std::is_same_v<Zip<MakePairT, TypeList<int, char, bool>, TypeList<float, double, long>>,
		                             TypeList<Pair<int, float>, Pair<char, double>, Pair<bool, long>>>);

	static_assert(std::is_same_v<Zip<std::common_type, TypeList<int, char, bool>, TypeList<unsigned, char16_t, bool>>,
		                         TypeList<unsigned, int, bool>>);

	static_assert(std::is_same_v<Map<std::add_lvalue_reference, TypeList<int, char, double>>,
		                         TypeList<int&, char&, double&>>);

	static_assert(std::is_same_v<Map<CloneT, TypeList<int, double>>,
		                         TypeList<Pair<int, int>, Pair<double, double>>>);
	
	static_assert(std::is_same_v<Map<HeadT, TypeList<TypeList<int, double>, TypeList<char, float>, TypeList<unsigned, short>>>,
		                         TypeList<int, char, unsigned>>);
	
	static_assert(std::is_same_v<Map<IdentityT, TypeList<int, float, double>>,
		                         TypeList<int, float, double>>);

	static_assert(std::is_same_v<LargestType<TypeList<char, unsigned, long double>>, long double>);

	static_assert(std::is_same_v<AccReverse<TypeList<int, double, char>>,
		                         TypeList<char, double, int>>);

	static_assert(std::is_same_v<AccConcat<TypeList<int, double>, TypeList<float, char>>,
		                         TypeList<int, double, float, char>>);

	static_assert(std::is_same_v<Filter<std::is_integral, TypeList<int, float, double, unsigned, bool>>,
		                         TypeList<int, unsigned, bool>>);

	static_assert(std::is_same_v<Filter<std::is_reference, TypeList<int, const int, const char, std::size_t, double>>,
		                         TypeList<>>);

	static_assert(std::is_same_v<Concat<TypeList<int, double>, TypeList<char, float>>,
		                          TypeList<int, double, char, float>>); 

	static_assert(std::is_same_v<Concat<TypeList<int, double>, TypeList<>>,
		                         TypeList<int, double>>);

	static_assert(std::is_same_v<FoldLeft<LargerT, char, TypeList<>>,
		                         char>);

	static_assert(std::is_same_v<FoldLeft<LargerT, char, TypeList<short, long, long double>>,
		                         long double>);

	static_assert(std::is_same_v<Drop<0, TypeList<int, float, double, char>>,
		                              TypeList<int, float, double, char>>);

	static_assert(std::is_same_v<Drop<3, TypeList<int, float, double, char>>,
		                         TypeList<char>>);

	static_assert(std::is_same_v<Take<3, TypeList<int, float, double, char>>,
		                         TypeList<int, float, double>>);

	static_assert(std::is_same_v<Take<0, TypeList<int, float, double, char>>,
		                         TypeList<>>);

	static_assert(std::is_same_v<First<SplitAt<0, TypeList<int>>>,
		                         TypeList<>>);

	static_assert(std::is_same_v<Second<SplitAt<0, TypeList<int>>>,
		                         TypeList<int>>);

	static_assert(std::is_same_v<First<SplitAt<1, TypeList<int, float, double>>>, 
		                         TypeList<int>>);

	static_assert(std::is_same_v<Second<SplitAt<1, TypeList<int, float, double>>>,
		                         TypeList<float, double>>);

	static_assert(std::is_same_v<MakeIndexList<0>, ValueList<unsigned>>);

	static_assert(std::is_same_v<MakeIndexList<4>, ValueList<unsigned, 0, 1, 2, 3>>);

	static_assert(std::is_same_v<ReplicateValue<10, 0>, ValueList<int>>);

	static_assert(std::is_same_v<ReplicateValue<10u, 2>, ValueList<unsigned, 10u, 10u>>);

	static_assert(std::is_same_v<InsertInSorted<int, TypeList<>, IsSmallerT>,
		                         TypeList<int>>);

	static_assert(std::is_same_v<InsertInSorted<int, TypeList<char, double>, IsSmallerT>,
		                         TypeList<char, int, double>>);

	static_assert(std::is_same_v<InsertionSort<TypeList<>, IsSmallerT>,
		                         TypeList<>>);

	static_assert(std::is_same_v<InsertionSort<TypeList<long, short, char, double>, IsSmallerT>,
		                         TypeList<char, short, long, double>>);

	static_assert(std::is_same_v<InsertionSort<ValueList<unsigned, 0, 1, 2>,
		                                       MakeIndexedCompareT<TypeList<double, char, int>, IsSmallerT>::template Compare>,
		                         ValueList<unsigned, 1, 2, 0>>);
	
	static_assert(allOf<std::is_const, TypeList<>>);

	static_assert(allOf<std::is_const, TypeList<const int, const double>>);

	static_assert(Inverse<std::is_const>::template Result<int>::value);

	static_assert(noneOf<std::is_const, TypeList<>>);

	static_assert(noneOf<std::is_const, TypeList<int, double>>);

	static_assert(!anyOf<std::is_const, TypeList<>>);

	static_assert(anyOf<std::is_const, TypeList<int, const double>>);
}