#include "http_flavour.hpp"
#include "unit.hpp"

HttpFlavour::HttpFlavour() = default;

HttpFlavour::~HttpFlavour() = default;

void HttpFlavour::prepare(Unit &unit)
{
}

void HttpFlavour::send(Unit &unit, std::string &request)
{
    ::send(unit.get_file_descriptor(), request.c_str(), request.length(), ::MSG_NOSIGNAL);
}

void HttpFlavour::receive(Unit &unit, char *buffer, std::size_t length)
{
    ::recv(unit.get_file_descriptor(), buffer, length, 0);
}

void HttpFlavour::connect(Unit &unit, const ::addrinfo &addrinfo)
{
    ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
}

HttpFlavour& HttpFlavour::instance()
{
    static HttpFlavour flavour;
    return flavour;
}

void HttpFlavour::close(Unit &unit)
{
    ::close(unit.get_file_descriptor());
}




