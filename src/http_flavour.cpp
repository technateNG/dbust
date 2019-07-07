#include "http_flavour.hpp"
#include "unit.hpp"

dbust::flavours::HttpFlavour::HttpFlavour() = default;

dbust::flavours::HttpFlavour::~HttpFlavour() = default;

int dbust::flavours::HttpFlavour::prepare(dbust::models::Unit& unit)
{
    return 0;
}

int dbust::flavours::HttpFlavour::send(dbust::models::Unit& unit, const std::string& request)
{
    auto s_bytes = ::send(
            unit.get_file_descriptor(),
            request.c_str(),
            request.length(),
            ::MSG_NOSIGNAL
    );
    if (!s_bytes)
    {
        return -1;
    }
    return 0;
}

int dbust::flavours::HttpFlavour::receive(dbust::models::Unit& unit, char* buffer, std::size_t length)
{
    auto r_bytes = ::recv(unit.get_file_descriptor(), buffer, length, 0);
    if (!r_bytes)
    {
        return -1;
    }
    return 0;
}

int dbust::flavours::HttpFlavour::connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo)
{
    auto s_conn = ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
    if (s_conn < 0)
    {
        return -1;
    }
    return 0;
}

dbust::flavours::HttpFlavour& dbust::flavours::HttpFlavour::instance()
{
    static HttpFlavour flavour;
    return flavour;
}

int dbust::flavours::HttpFlavour::close(dbust::models::Unit& unit)
{
    ::close(unit.get_file_descriptor());
    return 0;
}




