#include "CppUnitTest.h"
#include "Tuples\Tuple.h"
#include "Tuples\TupleAlgorithms.h"

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
			auto tuple = Tuple<int>{};

			Assert::AreEqual(tuple.getHead(), 0);
		}

		TEST_METHOD(variadicConstructorUsesPerfectForwarding)
		{
			const auto x = 1;
			auto str = "str"s;

			auto tuple = makeTuple(x, std::move(str));

			Assert::IsTrue(str == "", L"Argument is not moved");
			Assert::AreEqual(tuple.getHead(), x, L"Invalid head value");
			Assert::IsTrue(tuple.getTail().getHead() == "str", L"Invalid tail value");
		}

		TEST_METHOD(conversionConstructorFromLValue)
		{
			using Dest = Tuple<long, unsigned long>;
			const auto source = makeTuple(1, 2u);
			
			auto tuple = Dest(source);

			Assert::IsTrue(tuple == makeTuple(1l, 2ul));
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

		TEST_METHOD(copyConstructor)
		{
			auto source = makeTuple(1, 2u);
			auto destination = source;

			Assert::IsTrue(source == destination);
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
			auto source = makeTuple(1, 2);
			
			auto destination = Tuple<int,int>(source.getHead(), source.getTail());

			Assert::IsTrue(source == destination);
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

		TEST_METHOD(extractingValuesWithGet)
		{
			const auto tuple = makeTuple(1, 2, 3);

			auto x = get<0>(tuple);
			auto y = get<1>(tuple);

			Assert::AreEqual(x, 1);
			Assert::AreEqual(y, 2);
		}

		TEST_METHOD(getTakesValueCategoryIntoAccount)
		{
			auto tuple = makeTuple("a"s);

			auto str = get<0>(std::move(tuple));

			Assert::IsTrue(str == "a"s);
			Assert::IsTrue(tuple.getHead() == ""s);
		}

		TEST_METHOD(insertingBack)
		{
			auto tuple = insertBack(makeTuple(1, 2), 3);

			Assert::IsTrue(tuple == makeTuple(1, 2, 3));
		}

		TEST_METHOD(insertingFront)
		{
			auto tuple = insertFront(makeTuple(1, 2), 0);

			Assert::IsTrue(tuple == makeTuple(0, 1, 2));
		}

		TEST_METHOD(droppingTheTail)
		{
			auto tuple = dropTail(makeTuple(0, 1));

			Assert::IsTrue(tuple == makeTuple(0));
		}

		TEST_METHOD(droppingTheHead)
		{
			auto tuple = dropHead(makeTuple(0, 1, 2));

			Assert::IsTrue(tuple == makeTuple(1, 2));
		}

		TEST_METHOD(selectingElements)
		{
			auto source = makeTuple(2, 5, 4, 7, 8);

			auto odds = select<1, 3>(source);

			Assert::IsTrue(odds == makeTuple(5, 7));
		}

		TEST_METHOD(reversingATuple)
		{
			auto source = makeTuple(0, 1, 2, 3);
			
			auto result = reverse(source);

			Assert::IsTrue(result == makeTuple(3, 2, 1, 0));
		}

		TEST_METHOD(replicatingASingleValue)
		{
			auto source = makeTuple(0, 1, 2);

			auto result = replicate<1, 4>(source);

			Assert::IsTrue(result == makeTuple(1, 1, 1, 1));
		}

		TEST_METHOD(takingATuplePrefix)
		{
			auto source = makeTuple(0, 1, 2, 3, 4);

			auto result = take<2>(source);

			Assert::IsTrue(result == makeTuple(0, 1));
		}

		TEST_METHOD(takingATupleSuffix)
		{
			auto source = makeTuple(0, 1, 2, 3, 4);

			auto result = drop<2>(source);

			Assert::IsTrue(result == makeTuple(2, 3, 4));
		}

		TEST_METHOD(sortingATupleByType)
		{
			auto tuple = makeTuple(2, '1', 3.0);

			auto result = sortByType<IsSmallerT>(tuple);
		
			Assert::IsTrue(result == makeTuple('1', 2, 3.0));
		}

		TEST_METHOD(applyingAFunctionForEachElementOfATuple)
		{
			auto tuple = makeTuple(1, 2);
			auto plusOne = [](auto& x) { x += 1; };

			forEach(tuple, plusOne);

			Assert::IsTrue(tuple == makeTuple(2, 3));
		}

		TEST_METHOD(applyingAFunctionToTheElementsOfATuple)
		{
			auto tuple = makeTuple(1, 2);
			auto areEqual = [](auto x, auto y) { return x == y; };

			Assert::IsFalse(apply(areEqual, tuple));
		}
	};
}