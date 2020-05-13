#include <catch2/catch.hpp>
#include <vector>
#include "config.hpp"

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

struct NullDictionaryReader : public dbust::DictionaryReader {
    std::vector<std::string> read_dictionary(std::string_view path) const override
    {
        return std::vector<std::string>();
    }
};

TEST_CASE("CmdParser should function properly", "[CmdParser]")
{
    NullDictionaryReader nr;
    dbust::BatchOptParser bop;
    dbust::CmdParser cmd_parser(nr, bop);

    int m_argc { 16 };
    const char* m_argv[] {
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
        REQUIRE(config.get_status_codes() == dbust::StatusCodes { "200", "201" });
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

TEST_CASE("Target should correctly parse URL", "[Target]")
{
    const char* url_1 = "http://www.example.com:9005/foo/bar";
    const char* url_2 = "https://www.example.com:9003/foo";
    const char* url_3 = "www.example.com";
    const char* url_4 = "http://%31459amc~!/";
    const char* url_5 = "http:/\\www.example.com/";
    const char* url_6 = "http://www.example.com:zyx/";
    const char* url_7 = "http://127.0.0.1";
    const char* url_8 = "https://127.0.0.1";
    const char* url_9 = "https://192.168.1.12:9001/foobaroneoneone";

    SECTION("constructor should return correct Target instance with http")
    {
        dbust::Target tar_1 = dbust::Target::parse_url(url_1);
        REQUIRE(tar_1.get_host() == "www.example.com");
        REQUIRE(tar_1.get_port() == "9005");
        REQUIRE(tar_1.get_resource_path() == "/foo/bar/");
        REQUIRE_FALSE(tar_1.is_ssl());
    }

    SECTION("constructor should return correct Target instance with https")
    {
        dbust::Target tar_2 = dbust::Target::parse_url(url_2);
        REQUIRE(tar_2.get_host() == "www.example.com");
        REQUIRE(tar_2.get_port() == "9003");
        REQUIRE(tar_2.get_resource_path() == "/foo/");
        REQUIRE(tar_2.is_ssl());
    }

    SECTION("constructor should throw exception when url contains insufficient info")
    {
        REQUIRE_THROWS_AS(dbust::Target::parse_url(url_3), dbust::IncorrectUrlException);
    }

    SECTION("constructor should parse even strange urls")
    {
        dbust::Target tar_4 = dbust::Target::parse_url(url_4);
        REQUIRE(tar_4.get_host() == "%31459amc~!");
        REQUIRE(tar_4.get_port() == "80");
        REQUIRE(tar_4.get_resource_path() == "/");
        REQUIRE_FALSE(tar_4.is_ssl());
    }

    SECTION("constructor should throw exception when url appears correct but it isn't")
    {
        REQUIRE_THROWS_AS(dbust::Target::parse_url(url_5), dbust::IncorrectUrlException);
    }

    SECTION("constructor should throw exception when port in url is malformed")
    {
        REQUIRE_THROWS_AS(dbust::Target::parse_url(url_6), dbust::IncorrectUrlException);
    }

    SECTION("constructor should parse numeric http url")
    {
        dbust::Target tar_5 = dbust::Target::parse_url(url_7);
        REQUIRE(tar_5.get_host() == "127.0.0.1");
        REQUIRE(tar_5.get_port() == "80");
        REQUIRE(tar_5.get_resource_path() == "/");
        REQUIRE_FALSE(tar_5.is_ssl());
    }

    SECTION("constructor should parse numeric https url")
    {
        dbust::Target tar_5 = dbust::Target::parse_url(url_8);
        REQUIRE(tar_5.get_host() == "127.0.0.1");
        REQUIRE(tar_5.get_port() == "443");
        REQUIRE(tar_5.get_resource_path() == "/");
        REQUIRE(tar_5.is_ssl());
    }

    SECTION("constructor should parse normal numeric url")
    {
        dbust::Target tar_5 = dbust::Target::parse_url(url_9);
        REQUIRE(tar_5.get_host() == "192.168.1.12");
        REQUIRE(tar_5.get_port() == "9001");
        REQUIRE(tar_5.get_resource_path() == "/foobaroneoneone/");
        REQUIRE(tar_5.is_ssl());
    }
}
