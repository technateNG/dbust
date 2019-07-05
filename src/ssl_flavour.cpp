#include "ssl_flavour.hpp"
#include "unit.hpp"

dbust::flavours::SslFlavour::SslFlavour() = default;

void dbust::flavours::SslFlavour::prepare(dbust::models::Unit& unit)
{
    SSL_set_fd(unit.get_ssl_ptr(), unit.get_file_descriptor());
}

void dbust::flavours::SslFlavour::connect(
        dbust::models::Unit& unit,
        const ::addrinfo& addrinfo
)
{
    auto s_conn = ::connect(
            unit.get_file_descriptor(),
            addrinfo.ai_addr,
            addrinfo.ai_addrlen
    );
    if (s_conn < 0)
    {
        std::cerr << "[!] Can't connect " << unit.get_path() << " to target.  Error message: " <<
                  strerror(errno) << ". Error num: " << std::to_string(errno) << ". Reconnect issued." << std::endl;
        unit.set_state(dbust::models::Unit::State::DICONNECTED);
        return;
    }
    s_conn = ::SSL_connect(unit.get_ssl_ptr());
    if (s_conn < 0)
    {
        std::cerr << "[!] Can't establish SSL connection to target. Reconnect issued." << std::endl;
        unit.set_state(dbust::models::Unit::State::DICONNECTED);
    }
}

void dbust::flavours::SslFlavour::send(
        dbust::models::Unit& unit,
        std::string& request
)
{
    auto s_bytes = ::SSL_write(unit.get_ssl_ptr(), request.c_str(), request.length());
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

void dbust::flavours::SslFlavour::receive(
        dbust::models::Unit& unit,
        char* buffer,
        const std::size_t length
)
{
    auto r_bytes = ::SSL_read(unit.get_ssl_ptr(), buffer, length);
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

dbust::flavours::SslFlavour& dbust::flavours::SslFlavour::instance()
{
    static SslFlavour flavour;
    return flavour;
}

void dbust::flavours::SslFlavour::close(dbust::models::Unit& unit)
{
    ::SSL_shutdown(unit.get_ssl_ptr());
    ::SSL_clear(unit.get_ssl_ptr());
    ::close(unit.get_file_descriptor());
}

