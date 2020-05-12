#include "unit.hpp"
#include "connection_flavour.hpp"
#include <catch2/catch.hpp>

struct NullFlavour : public dbust::ConnectionFlavour {
    int send(dbust::Unit& unit, const std::string_view& request) const override
    {
        return error ? -1 : 0;
    }

    int receive(dbust::Unit& unit, char* buffer, std::size_t length) const override
    {
        return error ? -1 : 0;
    }

    int connect(dbust::Unit& unit, const ::addrinfo& addrinfo) const override
    {
        return error ? -1 : 0;
    }

    int close(dbust::Unit& unit) const override
    {
        return error ? -1 : 0;
    }

    int prepare(dbust::Unit& unit) const override
    {
        return error ? -1 : 0;
    }

    explicit NullFlavour(bool error)
        : error { error }
    {
    }

private:
    bool error;
};

TEST_CASE("Unit class behaves properly", "[Unit]")
{
    SECTION("after valid connection instance should have EMPTY state")
    {
        NullFlavour nf(false);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        addrinfo addr {};
        unit.connect(addr);
        REQUIRE(unit.get_state() == dbust::Unit::State::EMPTY);
    }

    SECTION("after invalid connection instance should have DISCONNECTED state")
    {
        NullFlavour nf(true);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        addrinfo addr {};
        unit.connect(addr);
        REQUIRE(unit.get_state() == dbust::Unit::State::DISCONNECTED_WITH_ERROR);
    }

    SECTION("after valid send instance should have SENDED state")
    {
        NullFlavour nf(false);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        std::string_view rqst = "foobar";
        unit.send(rqst);
        REQUIRE(unit.get_state() == dbust::Unit::State::SENDED);
    }

    SECTION("after invalid send instance should have BROKEN state")
    {
        NullFlavour nf(true);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        std::string_view rqst = "foobar";
        unit.send(rqst);
        REQUIRE(unit.get_state() == dbust::Unit::State::BROKEN);
    }

    SECTION("after valid receive instance should have EMPTY state")
    {
        NullFlavour nf(false);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        char* buff;
        unit.receive(buff, 10);
        REQUIRE(unit.get_state() == dbust::Unit::State::EMPTY);
    }

    SECTION("after intialisation instance should have 0 milis after epoch in timeout_tp member")
    {
        NullFlavour nf(false);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        REQUIRE(unit.get_timeout_tp() == std::chrono::system_clock::from_time_t(0));
    }

    SECTION("after initialisation instance should have 0 milis after epoch in delay_tp member")
    {
        NullFlavour nf(false);
        ::pollfd poll {};
        dbust::Unit unit(nf, poll);
        REQUIRE(unit.get_delay_tp() == std::chrono::system_clock::from_time_t(0));
    }
}