#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <unordered_set>
#include "utils.hpp"

TEST_CASE("Function is_in_status_codes() should return correct values.", "[utils]")
{
    std::unordered_set<std::string> status_codes {"200", "201", "401"};
    REQUIRE(dbust::utils::is_in_status_codes("400", status_codes));
    REQUIRE_FALSE(dbust::utils::is_in_status_codes("404", status_codes));
}