#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "response.hpp"
#include "status_codes.hpp"
#include "target.hpp"
#include "exceptions.hpp"


TEST_CASE("StatusCodes and Response should cooperate to get correct match", "[Request,StatusCodes]")
{
    dbust::models::StatusCodes sc { "200", "201" };
    const char c_buff[13] = "HTTP/1.1 200";
    const char b_buff[13] = "HTTP/1.1 404";
    dbust::models::Response c_resp(c_buff);
    dbust::models::Response b_resp(b_buff);

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

TEST_CASE("Target should correctly parse URL", "[Target]")
{
    const char* url_1 = "http://www.example.com:9005/foo/bar";
    const char* url_2 = "https://www.example.com:9003/foo";
    const char* url_3 = "www.example.com";
    const char* url_4 = "http://%31459amc~!/";
    const char* url_5 = "http:/\\www.example.com/";
    const char* url_6 = "http://www.example.com:zyx/";

    SECTION("constructor should return correct Target instance with http")
    {
        dbust::models::Target tar_1(url_1);
        REQUIRE(tar_1.get_host() == "www.example.com");
        REQUIRE(tar_1.get_port() == "9005");
        REQUIRE(tar_1.get_resource_path() == "/foo/bar/");
        REQUIRE_FALSE(tar_1.is_ssl());
    }

    SECTION("constructor should return correct Target instance with https")
    {
        dbust::models::Target tar_2(url_2);
        REQUIRE(tar_2.get_host() == "www.example.com");
        REQUIRE(tar_2.get_port() == "9003");
        REQUIRE(tar_2.get_resource_path() == "/foo/");
        REQUIRE(tar_2.is_ssl());
    }

    SECTION("constructor should throw exception when url contains insufficient info")
    {
        REQUIRE_THROWS_AS(dbust::models::Target(url_3), dbust::exceptions::IncorrectUrlException);
    }

    SECTION("constructor should parse even strange urls")
    {
        dbust::models::Target tar_4(url_4);
        REQUIRE(tar_4.get_host() == "%31459amc~!");
        REQUIRE(tar_4.get_port() == "80");
        REQUIRE(tar_4.get_resource_path() == "/");
        REQUIRE_FALSE(tar_4.is_ssl());
    }

    SECTION("constructor should throw exception when url appears correct but it isn't")
    {
        REQUIRE_THROWS_AS(dbust::models::Target(url_5), dbust::exceptions::IncorrectUrlException);
    }

    SECTION("constructor should throw exception when port in url is malformed")
    {
        REQUIRE_THROWS_AS(dbust::models::Target(url_6), dbust::exceptions::IncorrectUrlException);
    }
}