#include "http_flavour.hpp"
#include "unit.hpp"

dbust::flavours::HttpFlavour::HttpFlavour() = default;

dbust::flavours::HttpFlavour::~HttpFlavour() = default;

void dbust::flavours::HttpFlavour::prepare(dbust::models::Unit& unit)
{
}

void dbust::flavours::HttpFlavour::send(dbust::models::Unit& unit, std::string& request)
{
    auto s_bytes = ::send(
            unit.get_file_descriptor(),
            request.c_str(),
            request.length(),
            ::MSG_NOSIGNAL
    );
    if (!s_bytes)
    {
        std::cerr << "[!] Can't send data to: " << unit.get_path() <<
                  ". Reconnect issued." << std::endl;
        unit.set_state(dbust::models::Unit::State::BROKEN);
    } else
    {
        unit.set_state(dbust::models::Unit::State::SENDED);
    }
}

void dbust::flavours::HttpFlavour::receive(dbust::models::Unit& unit, char* buffer, std::size_t length)
{
    auto r_bytes = ::recv(unit.get_file_descriptor(), buffer, length, 0);
    if (!r_bytes)
    {
        std::cerr << "[!] Can't recv data from: " << unit.get_path() <<
                  ". Reconnect issued." << std::endl;
        unit.set_state(dbust::models::Unit::State::BROKEN);
    } else
    {
        unit.set_state(dbust::models::Unit::State::EMPTY);
    }
}

void dbust::flavours::HttpFlavour::connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo)
{
    auto s_conn = ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
    if (s_conn < 0)
    {
        std::cerr << "[!] Can't connect to target. Error message: " <<
                  strerror(errno) << ". Reconnect issued." << std::endl;
        unit.set_state(dbust::models::Unit::State::DICONNECTED);
    }
}

dbust::flavours::HttpFlavour& dbust::flavours::HttpFlavour::instance()
{
    static HttpFlavour flavour;
    return flavour;
}

void dbust::flavours::HttpFlavour::close(dbust::models::Unit& unit)
{
    ::close(unit.get_file_descriptor());
}




