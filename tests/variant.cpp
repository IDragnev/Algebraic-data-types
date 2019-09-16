#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Variant.h"
#include <string>

using IDragnev::Variant;
using IDragnev::visit;

TEST_CASE("the default constructor default-constructs the first type")
{
    struct NoDefault
    {
        NoDefault() = delete;
        NoDefault(double) { }
    };

    Variant<int, NoDefault> v;

    CHECK(v.is<int>());
    CHECK(v.get<int>() == 0);
}

TEST_CASE("constructing Variant from a value of any of the types")
{
    SUBCASE("basics")
    {
        Variant<int, double> v(2.0);

        CHECK(v.is<double>());
        CHECK(v.get<double>() == 2.0);
    }

    SUBCASE("implicit type convesions")
    {
        Variant<int, std::string> v("str");

        CHECK(v.is<std::string>());
        CHECK(v.get<std::string>() == "str");
    }
}

TEST_CASE("testing the move constructor")
{
    Variant<int, std::string> source("str");

    auto destination = std::move(source);

    REQUIRE(source.is<std::string>());
    REQUIRE(destination.is<std::string>());
    CHECK(source.get<std::string>() == "");
    CHECK(destination.get<std::string>() == "str");
}

TEST_CASE("testing the copy constructor")
{
    const Variant<int, std::string> source("str");

    auto destination = source;

    REQUIRE(source.is<std::string>());
    REQUIRE(destination.is<std::string>());
    CHECK(source.get<std::string>() == "str");
    CHECK(destination.get<std::string>() == "str");
}

TEST_CASE("testing the conversion constructors")
{
    struct X
    {
        X(std::string value) : value(value) { }
        X& operator=(std::string value) { this->value = value; }

        std::string value;
    };

    Variant<int, std::string> source("abc");

    SUBCASE("copy conversion")
    {
        Variant<int, float, X> destination = source;

        REQUIRE(source.is<std::string>());
        REQUIRE(destination.is<X>());
        CHECK(source.get<std::string>() == "abc");
        CHECK(destination.get<X>().value == "abc");
    }

    SUBCASE("move conversion")
    {
        Variant<int, float, X> destination = std::move(source);

        REQUIRE(source.is<std::string>());
        REQUIRE(destination.is<X>());
        CHECK(source.get<std::string>() == "");
        CHECK(destination.get<X>().value == "abc");
    }
}

TEST_CASE("testing the assignment of a plain value")
{
    SUBCASE("basics")
    {
        Variant<int, std::string> v("str");

        v = 100;

        REQUIRE(v.is<int>());
        CHECK(v.get<int>() == 100);
    }

    SUBCASE("type conversions")
    {
        Variant<int, std::string> v(100);

        v = "c-string";

        REQUIRE(v.is<std::string>());
        CHECK(v.get<std::string>() == "c-string");
    }
}

TEST_CASE("testing copy assignment")
{
    const Variant<int, double> rhs(10);
    Variant<double, int, float> lhs(0.0);

    lhs = rhs;

    REQUIRE(lhs.is<int>());
    REQUIRE(lhs.get<int>() == 10);
}

TEST_CASE("testing move assignment")
{
    Variant<int, std::string> rhs("abc");
    Variant<double, std::string> lhs(0.0);

    lhs = std::move(rhs);

    REQUIRE(lhs.is<std::string>());
    REQUIRE(rhs.is<std::string>());
    CHECK(lhs.get<std::string>() == "abc");
    CHECK(rhs.get<std::string>() == "");
}

TEST_CASE("testing the conversion assignment operators")
{
    struct X
    {
        X(std::string value) : value(value) {}
        X& operator=(std::string value) { this->value = value; }

        std::string value;
    };

    Variant<double, int, X> lhs(123.42);
    Variant<int, std::string> rhs("abc");

    SUBCASE("copy assignment")
    {
        lhs = std::as_const(rhs);

        REQUIRE(lhs.is<X>());
        CHECK(lhs.get<X>().value == "abc");
    }

    SUBCASE("move assignment")
    {
        lhs = std::move(rhs);

        REQUIRE(lhs.is<X>());
        REQUIRE(rhs.is<std::string>());
        CHECK(lhs.get<X>().value == "abc");
        CHECK(rhs.get<std::string>() == "");
    }
}

TEST_CASE("testing visit")
{
    SUBCASE("visiting a const variant")
    {
        const Variant<int, std::string> v("abc");
        std::string str = "";
        
        visit(v, [&str](const auto& value) { str = value; });

        CHECK(str == "abc");
    }

    SUBCASE("visiting a non-const variant")
    {
        Variant<int, double> v(10);

        auto result = visit(v, [](auto value) { return value; });

        CHECK(result == 10);
    }

    SUBCASE("visiting an rvalue variant")
    {
        Variant<int, double> v(20.0);
        
        auto result = visit(std::move(v), [](auto&& value) { return std::move(value); });

        CHECK(result == 20.0);
    }

    SUBCASE("specifying the return type for visit")
    {
        const Variant<int, float> v(10);
        
        auto result = visit<double>(v, [](auto x) { return x; });

        CHECK(result == 10.0);
    }
}


