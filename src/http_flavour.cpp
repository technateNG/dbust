#include "http_flavour.hpp"
#include "unit.hpp"

dbust::HttpFlavour::HttpFlavour() = default;

dbust::HttpFlavour::~HttpFlavour() = default;

int dbust::HttpFlavour::prepare(dbust::Unit& unit) const
{
    return 0;
}

int dbust::HttpFlavour::send(dbust::Unit& unit, const std::string_view& request) const
{
    auto s_bytes = ::send(
            unit.get_file_descriptor(),
            request.data(),
            request.length(),
            ::MSG_NOSIGNAL
    );
    if (!s_bytes)
    {
        return -1;
    }
    return 0;
}

int dbust::HttpFlavour::receive(dbust::Unit& unit, char* buffer, std::size_t length) const
{
    auto r_bytes = ::recv(unit.get_file_descriptor(), buffer, length, 0);
    if (!r_bytes)
    {
        return -1;
    }
    return 0;
}

int dbust::HttpFlavour::connect(dbust::Unit& unit, const ::addrinfo& addrinfo) const
{
    auto s_conn = ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
    if (s_conn < 0)
    {
        return (errno == EINPROGRESS) ? -3 : -1;
    }
    return 0;
}

dbust::HttpFlavour& dbust::HttpFlavour::instance()
{
    static HttpFlavour flavour;
    return flavour;
}

int dbust::HttpFlavour::close(dbust::Unit& unit) const
{
    ::close(unit.get_file_descriptor());
    return 0;
}




