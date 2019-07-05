#include "http_flavour.hpp"
#include "unit.hpp"

HttpFlavour::HttpFlavour() = default;

HttpFlavour::~HttpFlavour() = default;

void HttpFlavour::prepare(Unit &unit)
{
}

void HttpFlavour::send(Unit &unit, std::string &request)
{
    auto s_bytes = ::send(unit.get_file_descriptor(), request.c_str(), request.length(), ::MSG_NOSIGNAL);
    if (!s_bytes)
    {
        std::cerr << "[!] Can't send data to: " << unit.get_path() << ". Reconnect issued." << std::endl;
        unit.set_state(Unit::State::BROKEN);
    }
    else
    {
        unit.set_state(Unit::State::SENDED);
    }
}

void HttpFlavour::receive(Unit &unit, char *buffer, std::size_t length)
{
    auto r_bytes = ::recv(unit.get_file_descriptor(), buffer, length, 0);
    if (!r_bytes)
    {
        std::cerr << "[!] Can't recv data from: " << unit.get_path() << ". Reconnect issued." << std::endl;
        unit.set_state(Unit::State::BROKEN);
    }
    else
    {
        unit.set_state(Unit::State::EMPTY);
    }
}

void HttpFlavour::connect(Unit &unit, const ::addrinfo &addrinfo)
{
    auto s_conn = ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
    if (s_conn < 0)
    {
        std::cerr << "[!] Can't connect to target. Error message: " <<
        strerror(errno) << ". Reconnect issued." << std::endl;
        unit.set_state(Unit::State::DICONNECTED);
    }
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




