#include "Tuples\Tuple.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Meta::Tuple;
using IDragnev::Meta::makeTuple;
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
	};
}