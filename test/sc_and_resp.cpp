#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "response.hpp"
#include "status_codes.hpp"
#include "target.hpp"
#include "exceptions.hpp"


TEST_CASE("StatusCodes and Response should cooperate to get correct match", "[Request,StatusCodes]")
{
    dbust::StatusCodes sc { "200", "201" };
    const char c_buff[13] = "HTTP/1.1 200";
    const char b_buff[13] = "HTTP/1.1 404";
    dbust::Response c_resp(c_buff);
    dbust::Response b_resp(b_buff);

    SECTION("StatusCodes.contains should return correct value")
    {
        REQUIRE_FALSE(sc.contains("404"));
        REQUIRE(sc.contains("200"));
    }

    SECTION("Response should return correct status code")
    {
        REQUIRE(std::string_view(c_resp.get_status_code_ptr()) == "200");
        REQUIRE(std::string_view(b_resp.get_status_code_ptr()) == "404");
    }

    SECTION("should get correct value in main context")
    {
        REQUIRE(sc.contains(c_resp.get_status_code_ptr()));
        REQUIRE_FALSE(sc.contains(b_resp.get_status_code_ptr()));
    }
}

