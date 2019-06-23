#include "CppUnitTest.h"
#include "Tuples\Tuple.h"
#include "Tuples\TupleAlgorithms.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::Meta;
using namespace std::string_literals;

namespace Tupletest
{		
	TEST_CLASS(TupleTest)
	{
	public:
		TEST_METHOD(defaultConstructor)
		{
			constexpr auto tuple = Tuple<int>{};

			static_assert(get<0>(tuple) == 0);
		}

		TEST_METHOD(variadicConstructorAndMakeTupleUsePerfectForwarding)
		{
			const auto x = 1;
			auto str = "str"s;

			auto tuple = makeTuple(x, std::move(str));

			Assert::IsTrue(str == "", L"Argument is not moved");
			Assert::AreEqual(get<0>(tuple), x, L"Invalid head value");
			Assert::IsTrue(get<1>(tuple) == "str", L"Invalid tail value");
		}

		TEST_METHOD(variadicConstructorAndMakeTupleCanComputeAtCompileTime)
		{
			constexpr auto tuple = makeTuple(1, 2);
			
			static_assert(get<0>(tuple) == 1);
		}

		TEST_METHOD(extractingValuesWithGet)
		{
			constexpr auto tuple = makeTuple(1, 2);

			static_assert(get<0>(tuple) == 1);
			static_assert(get<1>(tuple) == 2);
		}

		TEST_METHOD(getTakesValueCategoryIntoAccount)
		{
			auto tuple = makeTuple("a"s);

			auto str = get<0>(std::move(tuple));

			Assert::IsTrue(str == "a"s);
			Assert::IsTrue(tuple.getHead() == ""s);
		}

		TEST_METHOD(conversionConstructorFromLValue)
		{
			using Dest = Tuple<long, unsigned long>;		
			constexpr auto source = makeTuple(1, 2u);
			
			constexpr auto tuple = Dest(source);

			static_assert(tuple == makeTuple(1l, 2ul));
		}

		TEST_METHOD(conversionConstructorFromRvalue)
		{
			class A
			{
			public:
				A(std::string&& x) : value(std::move(x)) { }
				
				bool operator==(const A& rhs) const { return value == rhs.value; }

			private:
				std::string value;
			};
			using Dest = Tuple<long, A>;

			auto source = makeTuple(1, "str"s);
			auto destination = Dest(std::move(source));

			Assert::IsTrue(source == makeTuple(1, ""s), L"Moved-from object has invalid constents");
			Assert::IsTrue(destination == makeTuple(1l, A("str"s)), L"Moved-in object has invalid contents");
		}

		TEST_METHOD(conversionConstructorFromRvalueCanComputeAtCompileTime)
		{
			using Dest = Tuple<long, double>;
			constexpr auto x = Dest(makeTuple(2, 2.f));

			static_assert(x == makeTuple(2l, 2.0));
		}

		TEST_METHOD(copyConstructor)
		{
			constexpr auto source = makeTuple(1, 2u);
			constexpr auto destination = source;

			static_assert(source == destination);
		}

		TEST_METHOD(moveConstructor)
		{
			auto source = makeTuple(1, "str"s);
			const auto expected = source;

			auto destination = std::move(source);

			Assert::IsTrue(destination == expected, L"Moved-in object has invalid contents");
			Assert::IsTrue(source == makeTuple(1, ""s), L"Moved-from object has invalid constents");
		}

		TEST_METHOD(headAndTailConstructor)
		{
			constexpr auto source = makeTuple(1, 2);
			
			constexpr auto destination = Tuple<int,int>(source.getHead(), source.getTail());

			static_assert(source == destination);
		}

		TEST_METHOD(copyAssignment)
		{
			auto rhs = makeTuple(1, 2u);
			auto lhs = makeTuple(0, 0u);

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(moveAssignment)
		{
			auto rhs = makeTuple(0, "str"s);
			auto lhs = makeTuple(0, ""s);
			const auto expectedLhs = rhs;
			const auto expectedRhs = lhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == expectedLhs, L"Moved-in object has invalid contents");
			Assert::IsTrue(rhs == expectedRhs, L"Moved-from object has invalid contents");
		}

		TEST_METHOD(conversionCopyAssignment)
		{
			auto lhs = makeTuple(0l, 1.0);
			auto rhs = makeTuple(1, 2.f);
			
			lhs = rhs;

			Assert::IsTrue(lhs == makeTuple(1l, 2.0));
		}

		TEST_METHOD(conversionMoveAssignment)
		{
			auto lhs = makeTuple(0l, ""s);
			auto rhs = makeTuple(1, "str"s);

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == makeTuple(1l, "str"s), L"Moved-in object has invalid contents");
			Assert::IsTrue(rhs == makeTuple(1, ""s), L"Moved-from object has invalid contents");
		}

		TEST_METHOD(insertingBack)
		{
			constexpr auto tuple = insertBack(makeTuple(1, 2), 3);

			static_assert(tuple == makeTuple(1, 2, 3));
		}

		TEST_METHOD(insertingFront)
		{
			constexpr auto tuple = insertFront(makeTuple(1, 2), 0);

			static_assert(tuple == makeTuple(0, 1, 2));
		}

		TEST_METHOD(droppingTheTail)
		{
			constexpr auto tuple = dropTail(makeTuple(0, 1));

			static_assert(tuple == makeTuple(0));
		}

		TEST_METHOD(droppingTheHead)
		{
			constexpr auto tuple = dropHead(makeTuple(0, 1, 2));

			static_assert(tuple == makeTuple(1, 2));
		}

		TEST_METHOD(selectingElements)
		{
			constexpr auto source = makeTuple(2, 5, 4, 7, 8);

			constexpr auto odds = select<1, 3>(source);

			static_assert(odds == makeTuple(5, 7));
		}

		TEST_METHOD(movingSubtuplesWithSelect)
		{
			auto source = makeTuple(2, "a"s, "b"s);

			auto destination = select<1, 2>(std::move(source));

			Assert::IsTrue(source == makeTuple(2, ""s, ""s));
			Assert::IsTrue(destination == makeTuple("a"s, "b"s));
		}

		TEST_METHOD(reversingATuple)
		{
			constexpr auto source = makeTuple(0, 1, 2, 3);
			
			constexpr auto result = reverse(source);

			static_assert(result == makeTuple(3, 2, 1, 0));
		}

		TEST_METHOD(reverseTakesValueCategoryIntoAccount)
		{
			auto source = makeTuple(1, "a"s);

			auto result = reverse(std::move(source));

			Assert::IsTrue(source == makeTuple(1, ""s));
			Assert::IsTrue(result == makeTuple("a"s, 1));
		}

		TEST_METHOD(replicatingASingleValue)
		{
			constexpr auto source = makeTuple(0, 1, 2);

			constexpr auto result = replicate<1, 4>(source);

			static_assert(result == makeTuple(1, 1, 1, 1));
		}

		TEST_METHOD(takingATuplePrefix)
		{
			constexpr auto source = makeTuple(0, 1, 2, 3, 4);

			constexpr auto result = take<2>(source);

			static_assert(result == makeTuple(0, 1));
		}

		TEST_METHOD(takeTakesValueCategoryIntoAccount)
		{
			auto source = makeTuple("a"s, "b"s, "c"s);

			auto result = take<2>(std::move(source));
			
			Assert::IsTrue(source == makeTuple(""s, ""s, "c"s));
			Assert::IsTrue(result == makeTuple("a"s, "b"s));
		}
		TEST_METHOD(takingATupleSuffix)
		{
			constexpr auto source = makeTuple(0, 1, 2, 3, 4);

			constexpr auto result = drop<2>(source);

			static_assert(result == makeTuple(2, 3, 4));
		}

		TEST_METHOD(dropTakesValueCategoryIntoAccount)
		{
			auto source = makeTuple("a"s, "b"s, "c"s);

			auto result = drop<2>(std::move(source));

			Assert::IsTrue(source == makeTuple("a"s, "b"s, ""s));
			Assert::IsTrue(result == makeTuple("c"s));
		}

		TEST_METHOD(sortingATupleByType)
		{
			constexpr auto tuple = makeTuple(2, '1', 3.0);

			constexpr auto result = sortByType<IsSmallerT>(tuple);
		
			static_assert(result == makeTuple('1', 2, 3.0));
		}

		TEST_METHOD(applyingAFunctionForEachElementOfATuple)
		{
			auto tuple = makeTuple(1, 2);
			auto plusOne = [](auto& x) { x += 1; };

			forEach(tuple, plusOne);

			Assert::IsTrue(tuple == makeTuple(2, 3));
		}

		TEST_METHOD(forEachTakesValueCategoryIntoAccount)
		{
			using Strings = std::vector<std::string>;

			auto result = Strings{};
			auto tuple = makeTuple("a"s, "b"s, "c"s);
			auto moveToResult = [&result](std::string&& x) { result.push_back(std::move(x)); };
			
			forEach(std::move(tuple), moveToResult);

			Assert::IsTrue(tuple == makeTuple(""s, ""s, ""s));
			Assert::IsTrue(result == Strings{ "a", "b", "c" });
		}

		TEST_METHOD(applyingAFunctionToTheElementsOfATuple)
		{
			constexpr auto tuple = makeTuple(1, 2);
			constexpr auto areEqual = [](auto x, auto y) constexpr { return x == y; };

			static_assert(!apply(areEqual, tuple));
		}

		TEST_METHOD(applyTakesValueCategoryIntoAccount)
		{
			auto tuple = makeTuple("a"s, "b"s);
			auto sum = [](const auto& x, auto&& y)
			{
				return x + std::forward<decltype(y)>(y);
			};

			auto result = IDragnev::Meta::apply(sum, std::move(tuple));

			Assert::IsTrue(result == "ab"s);
			Assert::IsTrue(tuple == makeTuple("a"s, ""s));
		}

		TEST_METHOD(concatenatingTuples)
		{
			constexpr auto lhs = makeTuple(1, 2, 3);
			constexpr auto rhs = makeTuple(4, 5);

			constexpr auto result = concatenate(lhs, rhs);

			static_assert(result == makeTuple(1, 2, 3, 4, 5));
		}

		TEST_METHOD(concatenateTakesValueCategoryIntoAccount)
		{
			const auto lhs = makeTuple(1, 2, 3);
			auto rhs = makeTuple(4, "a"s);

			auto result = concatenate(lhs, std::move(rhs));

			Assert::IsTrue(rhs == makeTuple(4, ""s));
			Assert::IsTrue(result == makeTuple(1, 2, 3, 4, "a"s));
		}

		TEST_METHOD(concatenateTakesAnyNumberOfTuplesGreaterThanOne)
		{
			constexpr auto first = makeTuple(1, 2);
			constexpr auto second = makeTuple(3, 4, 5);
			constexpr auto third = makeTuple(6);
			
			constexpr auto result = concatenate(first, second, third);

			static_assert(result == makeTuple(1, 2, 3, 4, 5, 6));
		}

		TEST_METHOD(tupleEquality)
		{
			static_assert(makeTuple(1, 2) == makeTuple(1, 2));
			static_assert(!(makeTuple(1, 2) != makeTuple(1, 2)));
		}

		TEST_METHOD(tupleGreaterThan)
		{
			static_assert(makeTuple(3, 4) > makeTuple(2, 3));
			static_assert(!(makeTuple(3, 4) > makeTuple(3, 1)));
		}

		TEST_METHOD(tupleLessThan)
		{
			static_assert(makeTuple(1, 4) < makeTuple(2, 5));
			static_assert(!(makeTuple(3, 4) < makeTuple(3, 10)));
		}
		TEST_METHOD(tupleGreaterOrEqual)
		{
			static_assert(makeTuple(3, 4) >= makeTuple(2, 4));
			static_assert(!(makeTuple(3, 4) >= makeTuple(2, 5)));
		}

		TEST_METHOD(tupleLessOrEqual)
		{
			static_assert(makeTuple(3, 4) <= makeTuple(3, 5));
			static_assert(!(makeTuple(3, 4) <= makeTuple(4, 3)));
		}

		TEST_METHOD(unpackingATuple)
		{
			auto x = 0;
			auto y = 0.0;
			const auto tuple = makeTuple(1, 2.0, "a"s);
			
			tie(x, y, ignore) = tuple;

			Assert::AreEqual(x, 1);
			Assert::AreEqual(y, 2.0);
		}
	};
}