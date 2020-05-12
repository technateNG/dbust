#include "batch_opt_parser.hpp"
#include <catch2/catch.hpp>

template <typename T>
bool contains(T pattern, std::vector<T>& vec)
{
    for (T& elem : vec) {
        if (elem == pattern) {
            return true;
        }
    }
    return false;
}

TEST_CASE("BatchOptParser", "[BatchOptParser]")
{
    dbust::BatchOptParser bop;
    std::string a = "foo,bar,zar";
    std::string b = "\"foo,bar,zar\"";
    std::string c = "\'foo,bar,zar\'";
    std::string d = "'foo,bar,zar";
    std::string e = "\"foo,bar,zar";
    std::string f = "foo,bar,zar,";
    std::string g = ",foo,bar,zar";

    SECTION("should parse raw opts correctly")
    {
        auto res = bop.parse(a.c_str());
        REQUIRE(contains<std::string>("foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }

    SECTION("should find correct amount of opts")
    {
        auto res = bop.parse(a.c_str());
        REQUIRE(res.size() == 3);
    }

    SECTION("should parse double quoted opts correctly")
    {
        auto res = bop.parse(b.c_str());
        REQUIRE(contains<std::string>("foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }

    SECTION("should parse single quoted opts correctly")
    {
        auto res = bop.parse(c.c_str());
        REQUIRE(contains<std::string>("foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }

    SECTION("should parse single quote on front opts correctly")
    {
        auto res = bop.parse(d.c_str());
        REQUIRE(contains<std::string>("'foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }

    SECTION("should parse double quote on front opts correctly")
    {
        auto res = bop.parse(e.c_str());
        REQUIRE(contains<std::string>("\"foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }

    SECTION("should parse comma on back opts correctly")
    {
        auto res = bop.parse(f.c_str());
        REQUIRE(contains<std::string>("foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
        REQUIRE(contains<std::string>("", res));
    }

    SECTION("should parse comma on front opts correctly")
    {
        auto res = bop.parse(g.c_str());
        REQUIRE(contains<std::string>("foo", res));
        REQUIRE(contains<std::string>("bar", res));
        REQUIRE(contains<std::string>("zar", res));
    }
}