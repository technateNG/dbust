#include <catch2/catch.hpp>
#include "cmd_parser.hpp"

struct NullDictionaryReader : public dbust::DictionaryReader
{
    std::vector<std::string> read_dictionary(const char* path) const override
    {
        return std::vector<std::string>();
    }
};

TEST_CASE("CmdParser should function properly", "[CmdParser]")
{
    NullDictionaryReader nr;
    dbust::BatchOptParser bop;
    dbust::CmdParser cmd_parser(nr, bop);

    int m_argc{ 16 };
    const char* m_argv[]
            {
                    "/home/foo/dbust",
                    "--sockets",
                    "1",
                    "--url",
                    "http://www.example.com/",
                    "--dictionary",
                    "/home/foo/dict",
                    "-c",
                    "'200,201'",
                    "-y",
                    "200",
                    "--timeout",
                    "12",
                    "--get",
                    "--user-agent",
                    "foobar"
            };

    SECTION("should return correct Config instance from opts")
    {
        dbust::Config config = cmd_parser.parse(m_argc, m_argv);
        REQUIRE(config.get_status_codes() == dbust::StatusCodes{ "200", "201" });
        REQUIRE(config.get_nb_of_sockets() == 1);
        REQUIRE(config.get_delay() == 200);
        REQUIRE(config.get_target().get_host() == "www.example.com");
        REQUIRE(config.get_target().get_port() == "80");
        REQUIRE_FALSE(config.get_target().is_ssl());
        REQUIRE(config.get_timeout() == 12);
        REQUIRE(config.is_get());
        REQUIRE(config.get_user_agent() == "foobar");
    }
}