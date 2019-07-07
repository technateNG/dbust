#include <catch2/catch.hpp>
#include "cmd_parser.hpp"

TEST_CASE("CmdParser should function properly", "[CmdParser]")
{
    dbust::models::CmdParser parser;
    int c_argc{7};
    const char* c_argv[]
            {
                    "/home/foo/dbust",
                    "-s",
                    "1",
                    "-u",
                    "http://www.example.com/",
                    "-d",
                    "/home/foo/dict",
                    "-c '200,201'"
            };

    SECTION("CmdParser should return correrct Config instance")
    {
  //      dbust::models::Config config = parser.parse(c_argc, c_argv);
 //       REQUIRE(config.get_status_codes() == dbust::models::StatusCodes{ "200", "201" });
    }
}