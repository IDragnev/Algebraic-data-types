#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Variant.h"
#include <string>

using IDragnev::Variant;

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
    Variant<int, std::string> source("str");

    auto destination = source;

    REQUIRE(source.is<std::string>());
    REQUIRE(destination.is<std::string>());
    CHECK(source.get<std::string>() == "str");
    CHECK(destination.get<std::string>() == "str");
}

