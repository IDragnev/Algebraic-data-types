#pragma once

#include "List algorithms.h"
#include "TypeList.h"

namespace IDragnev::Meta
{
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

	static_assert(std::is_same_v<Drop<3, TypeList<int, float, double, char>>,
		                         TypeList<char>>);
}