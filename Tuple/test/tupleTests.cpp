#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Tuple.h"
#include "TupleIO.h"
#include "TupleAlgorithms.h"
#include "Meta/Tests.h"
#include <vector>

using namespace IDragnev;
using namespace IDragnev::TupleAlgorithms;
using namespace std::string_literals;

namespace idr = IDragnev;
namespace alg = idr::TupleAlgorithms;

TEST_CASE("default constructor")
{
    constexpr auto tuple = Tuple<int>{};

    static_assert(get<0>(tuple) == 0);
}

TEST_CASE("variadic constructor and makeTuple")
{
    SUBCASE("they both use perfect forwarding")
    {
        const auto x = 1;
        auto str = "str"s;

        auto tuple = makeTuple(x, std::move(str));

        CHECK(str == "");
        CHECK(get<0>(tuple) == x);
        CHECK(get<1>(tuple) == "str");
    }

    SUBCASE("they can both be used at compile time")
    {
        constexpr auto tuple = makeTuple(1, 2);

        static_assert(get<0>(tuple) == 1);
        static_assert(get<1>(tuple) == 2);
    }
}

TEST_CASE("get")
{
    SUBCASE("basics")
    {
        constexpr auto tuple = makeTuple(1, 2);

        static_assert(get<0>(tuple) == 1);
        static_assert(get<1>(tuple) == 2);
    }

    SUBCASE("get takes value category into account")
    {
        auto tuple = makeTuple("a"s);

        auto str = get<0>(std::move(tuple));

        CHECK(str == "a");
        CHECK(get<0>(tuple) == "");
    }
}

TEST_CASE("conversion constructors")
{
    SUBCASE("lvalue conversion")
    {
        constexpr auto source = makeTuple(1, 2u);

        using Dest = Tuple<long, unsigned long>;
        constexpr auto tuple = Dest(source);

        static_assert(tuple == makeTuple(1l, 2ul));
    }

    SUBCASE("rvalue conversion")
    {
        auto source = makeTuple(1, "str"s);

        using Dest = Tuple<long, std::string>;
        auto destination = Dest(std::move(source));

        CHECK(source == makeTuple(1, ""));
        CHECK(destination == makeTuple(1l, "str"));
    }

    SUBCASE("computing at compile time")
    {
        using Dest = Tuple<long, double>;
        constexpr auto x = Dest(makeTuple(2, 2.f));

        static_assert(x == makeTuple(2l, 2.0));
    }
}

TEST_CASE("copy constructor")
{
    constexpr auto source = makeTuple(1, 2u);
    constexpr auto destination = source;

    static_assert(source == destination);
}

TEST_CASE("move constructor")
{
    auto source = makeTuple(1, "str"s);
    const auto expected = source;

    const auto destination = std::move(source);

    CHECK(destination == expected);
    CHECK(source == makeTuple(1, ""));
}

TEST_CASE("copy assignment")
{
    SUBCASE("basics")
    {
        auto rhs = makeTuple(1, 2u);
        auto lhs = makeTuple(0, 0u);

        lhs = rhs;

        CHECK(lhs == rhs);
    }
   
    SUBCASE("computing at compile time")
    {
        constexpr auto f = []() constexpr
        {         
            return reverse(makeTuple(1, 2));
        };

        constexpr auto tuple = f();

        static_assert(tuple == makeTuple(2, 1));
    }
}

TEST_CASE("move assignment")
{
    auto lhs = makeTuple(0, ""s);
    auto rhs = makeTuple(0, "str"s);
    const auto expectedLhs = rhs;
    const auto expectedRhs = lhs;

    lhs = std::move(rhs);

    CHECK(lhs == expectedLhs);
    CHECK(rhs == expectedRhs);
}

TEST_CASE("conversion assignment operators")
{
    SUBCASE("copy assignment")
    {
        auto lhs = makeTuple(0l, 1.0);
        const auto rhs = makeTuple(1, 2.f);

        lhs = rhs;

        CHECK(lhs == makeTuple(1l, 2.0));
    }
    
    SUBCASE("move assignment")
    {
        auto lhs = makeTuple(0l, ""s);
        auto rhs = makeTuple(1, "str"s);

        lhs = std::move(rhs);

        CHECK(lhs == makeTuple(1l, "str"));
        CHECK(rhs == makeTuple(1, ""));
    }
}

TEST_CASE("insertBack")
{
    SUBCASE("insertBack accecpts any positive number of arguments to append")
    {
        auto tuple = insertBack(makeTuple(1, 2), 3, 4, 5);

        CHECK(tuple == makeTuple(1, 2, 3, 4, 5));
    }

    SUBCASE("insertBack takes the tuple's value category into account")
    {
        auto source = makeTuple("a"s, "b"s);

        const auto result = insertBack(std::move(source), 1);

        CHECK(source == makeTuple("", ""));
        CHECK(result == makeTuple("a", "b", 1));
    }

    SUBCASE("insertBack uses perfect forwarding on its arguments")
    {
        const auto source = makeTuple(1, 2);
        auto str = "a"s;

        const auto result = insertBack(source, std::move(str));
        
        CHECK(result == makeTuple(1, 2, "a"));
        CHECK(str == "");
    }

    SUBCASE("computing at compile time")
    {
        constexpr auto tuple = insertBack(makeTuple(1, 2), 3, 4, 5);

        static_assert(tuple == makeTuple(1, 2, 3, 4, 5));
    }
}

TEST_CASE("insertFront")
{
    SUBCASE("insertFront accecpts any positive number of arguments to prepend")
    {
        auto tuple = insertFront(makeTuple(3, 4, 5), 1, 2);

        CHECK(tuple == makeTuple(1, 2, 3, 4, 5));
    }

    SUBCASE("insertFront takes the tuple's value category into account")
    {
        auto source = makeTuple("a"s, "b"s);

        const auto result = insertFront(std::move(source), 1);

        CHECK(source == makeTuple("", ""));
        CHECK(result == makeTuple(1, "a", "b"));
    }

    SUBCASE("insertFront uses perfect forwarding on its arguments")
    {
        const auto source = makeTuple(1, 2);
        auto str = "0"s;

        const auto result = insertFront(source, std::move(str));

        CHECK(result == makeTuple("0", 1, 2));
        CHECK(str == "");
    }

    SUBCASE("computing at compile time")
    {
        constexpr auto tuple = insertFront(makeTuple(1, 2), 0);

        static_assert(tuple == makeTuple(0, 1, 2));
    }
}

TEST_CASE("dropTail")
{
    static_assert(dropTail(makeTuple(0, 1)) == makeTuple(0));
}

TEST_CASE("dropHead")
{
    static_assert(dropHead(makeTuple(0, 1, 2)) == makeTuple(1, 2));
}

TEST_CASE("select")
{
    SUBCASE("basics")
    {
        constexpr auto source = makeTuple(2, 5, 4, 7, 8, 9);

        constexpr auto odds = select<1, 3, 5>(source);

        static_assert(odds == makeTuple(5, 7, 9));
    }

    SUBCASE("select takes the tuple's value category into account")
    {
        auto source = makeTuple(2, "a"s, "b"s);

        auto destination = select<1, 2>(std::move(source));

        CHECK(source == makeTuple(2, "", ""));
        CHECK(destination == makeTuple("a", "b"));
    }
}

TEST_CASE("reverse")
{
    SUBCASE("basics")
    {
        constexpr auto tuple = makeTuple(0, 1, 2, 3);

        static_assert(reverse(tuple) == makeTuple(3, 2, 1, 0));
    }

    SUBCASE("reverse takes the tuple's value category into account")
    {
        auto source = makeTuple(1, "a"s);

        const auto result = reverse(std::move(source));

        CHECK(source == makeTuple(1, ""));
        CHECK(result == makeTuple("a", 1));
    }
}

TEST_CASE("replicate")
{
    constexpr auto source = makeTuple(0, 1, 2);

    constexpr auto result = replicate<1, 4>(source);

    static_assert(result == makeTuple(1, 1, 1, 1));
}

TEST_CASE("take")
{
    SUBCASE("basics")
    {
        constexpr auto source = makeTuple(0, 1, 2, 3, 4);

        constexpr auto result = take<2>(source);

        static_assert(result == makeTuple(0, 1));
    }

    SUBCASE("take takes the tuple's value category into account")
    {
        auto source = makeTuple("a"s, "b"s, "c"s);

        const auto result = take<2>(std::move(source));

        CHECK(source == makeTuple("", "", "c"));
        CHECK(result == makeTuple("a", "b"));
    }
}

TEST_CASE("drop")
{
    SUBCASE("basics")
    {
        constexpr auto source = makeTuple(0, 1, 2, 3, 4);

        constexpr auto result = drop<2>(source);

        static_assert(result == makeTuple(2, 3, 4));
    }

    SUBCASE("drop takes the tuple's value category into account")
    {
        auto source = makeTuple("a"s, "b"s, "c"s);

        const auto result = drop<2>(std::move(source));

        CHECK(source == makeTuple("a", "b", ""));
        CHECK(result == makeTuple("c"));
    }
}

TEST_CASE("sortByType")
{
    SUBCASE("basics")
    {
        constexpr auto tuple = makeTuple(2, '1', 3.0);

        constexpr auto result = sortByType<Meta::IsSmallerT>(tuple);

        static_assert(result == makeTuple('1', 2, 3.0));
    }

    SUBCASE("sort takes the tuple's value category into account")
    {
        auto source = makeTuple("1"s, 0);

        const auto result = sortByType<Meta::IsSmallerT>(std::move(source));

        CHECK(result == makeTuple(0, "1"));
        CHECK(source == makeTuple("", 0));
    }
}

TEST_CASE("forEach")
{
    SUBCASE("basics")
    {
        auto tuple = makeTuple(1, 2);
        auto plusOne = [](auto& x) { x += 1; };

        forEach(tuple, plusOne);

        CHECK(tuple == makeTuple(2, 3));
    }

    SUBCASE("forEach takes the tuple's value category into account"
            "when passing its components to the supplied function")
    {
        using Strings = std::vector<std::string>;

        auto result = Strings{};
        auto tuple = makeTuple("a"s, "b"s, "c"s);
        auto moveToResult = [&result](std::string&& x) { result.push_back(std::move(x)); };

        forEach(std::move(tuple), moveToResult);

        CHECK(tuple == makeTuple("", "", ""));
        CHECK(result == Strings{ "a", "b", "c" });
    }

    SUBCASE("computing at compile time")
    {
        constexpr auto f = []() constexpr
        {
            auto tuple = makeTuple(1, 2, 3);
            auto plusOne = [](auto& x) constexpr { x += 1; };
            
            forEach(tuple, plusOne);

            return tuple;
        };

        static_assert(f() == makeTuple(2, 3, 4));
    }
}

TEST_CASE("apply")
{
    SUBCASE("basics (and compile time computation)")
    {
        constexpr auto tuple = makeTuple(1, 2);
        constexpr auto areEqual = [](auto x, auto y) constexpr { return x == y; };

        static_assert(!apply(areEqual, tuple));
    }

    SUBCASE("apply takes the tuple's value category into account"
            "when passing its components to the supplied function")
    {
        auto tuple = makeTuple("a"s, "b"s);
        auto sum = [](auto x, auto y)
        {
            return std::move(x) + std::move(y);
        };

        auto result = alg::apply(sum, std::move(tuple));

        CHECK(result == "ab");
        CHECK(tuple == makeTuple("", ""));
    }
}

TEST_CASE("foldl")
{
    SUBCASE("basics (and compile time computation)")
    {
        constexpr auto tuple = makeTuple(1, 2, 3);

        constexpr auto result = foldl(tuple, 0, std::plus{});

        static_assert(result == 6);
    }

    SUBCASE("foldl takes the tuple's value category into account")
    {
        auto tuple = makeTuple("a"s, "b"s, "c"s);
        auto concat = [](auto acc, auto x) { return std::move(acc) + std::move(x); };

        const auto result = foldl(std::move(tuple), ""s, concat);

        CHECK(result == "abc");
        CHECK(tuple == makeTuple("", "", ""));
    }
}

TEST_CASE("transform")
{
    SUBCASE("basics and compile time usage")
    {
        constexpr auto plusOne = [](auto x) constexpr { return x + 1; };

        constexpr auto tuple = transform(makeTuple(1, 2), plusOne);

        static_assert(tuple == makeTuple(2, 3));
    }

    SUBCASE("transform take the tuple's value category into account")
    {
        const auto f = [](auto x) { return x + "!"; };
        auto source = makeTuple("a"s, "b"s);

        const auto result = transform(std::move(source), f);

        CHECK(result == makeTuple("a!", "b!"));
        CHECK(source == makeTuple("", ""));
    }
}

TEST_CASE("concatenate")
{
    SUBCASE("basics (and compile time computation)")
    {
        constexpr auto lhs = makeTuple(1, 2, 3);
        constexpr auto rhs = makeTuple(4, 5);

        static_assert(concatenate(lhs, rhs) == makeTuple(1, 2, 3, 4, 5));
    }

    SUBCASE("concatenate takes the tuples' value category into account")
    {
        const auto lhs = makeTuple(1, 2, 3);
        auto rhs = makeTuple(4, "a"s);

        const auto result = concatenate(lhs, std::move(rhs));

        CHECK(rhs == makeTuple(4, ""));
        CHECK(result == makeTuple(1, 2, 3, 4, "a"));
    }

    SUBCASE("concatenate takes any number of tuples (and at least two)")
    {
        constexpr auto first = makeTuple(1, 2);
        constexpr auto second = makeTuple(3, 4, 5);
        constexpr auto third = makeTuple(6);

        constexpr auto result = concatenate(first, second, third);

        static_assert(result == makeTuple(1, 2, 3, 4, 5, 6));
    }
}

TEST_CASE("tuple comparisions")
{
    static_assert(makeTuple(1, 2) == makeTuple(1, 2));
    static_assert(!(makeTuple(1, 2) != makeTuple(1, 2)));

    static_assert(makeTuple(3, 4) > makeTuple(2, 3));
    static_assert(!(makeTuple(3, 4) > makeTuple(3, 1)));

    static_assert(makeTuple(1, 4) < makeTuple(2, 5));
    static_assert(!(makeTuple(3, 4) < makeTuple(3, 10)));

    static_assert(makeTuple(3, 4) >= makeTuple(2, 4));
    static_assert(!(makeTuple(3, 4) >= makeTuple(2, 5)));

    static_assert(makeTuple(3, 4) <= makeTuple(3, 5));
    static_assert(!(makeTuple(3, 4) <= makeTuple(4, 3)));
}

TEST_CASE("tie")
{
    auto x = 0;
    auto y = 0.0;
    const auto tuple = makeTuple(1, 2.0, "a"s);

    tie(x, y, idr::ignore) = tuple;

    CHECK(x == 1);
    CHECK(y == 2.0);
}

TEST_CASE("pipe")
{    
    SUBCASE("basics")
    {
        constexpr auto sum = [](const auto& t) constexpr 
        { 
            return foldl(t, 0, std::plus{});
        };

        constexpr auto s = makeTuple(1, 2, 3, 4, 5)
                           | take<3>
                           | sum;
        static_assert(s == 6);
    }

    SUBCASE("the pipe uses perfect forwarding")
    {
        const auto tuple = makeTuple(1, 2, 3, 4) 
                           | take<3>
                           | reverse;

        CHECK(tuple == makeTuple(3, 2, 1));
    }
} 