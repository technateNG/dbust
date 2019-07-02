#include "ssl_flavour.hpp"
#include "unit.hpp"

dbust::SslFlavour::SslFlavour() = default;

int dbust::SslFlavour::prepare(dbust::Unit& unit) const
{
    SSL_set_fd(unit.get_ssl_ptr(), unit.get_file_descriptor());
    return 0;
}

int dbust::SslFlavour::connect(
        dbust::Unit& unit,
        const ::addrinfo& addrinfo
) const
{
    auto s_conn = ::connect(
            unit.get_file_descriptor(),
            addrinfo.ai_addr,
            addrinfo.ai_addrlen
    );
    if (s_conn < 0)
    {
        return (errno == EINPROGRESS) ? -3 : -1;
    }
    s_conn = ::SSL_connect(unit.get_ssl_ptr());
    if (s_conn < 0)
    {
        return -2;
    }
    return 0;
}

int dbust::SslFlavour::send(
        dbust::Unit& unit,
        const std::string_view& request
) const
{
    auto s_bytes = ::SSL_write(unit.get_ssl_ptr(), request.data(), request.length());
    if (s_bytes <= 0)
    {
        return -1;
    }
    return 0;
}

int dbust::SslFlavour::receive(
        dbust::Unit& unit,
        char* buffer,
        const std::size_t length
) const
{
    auto r_bytes = ::SSL_read(unit.get_ssl_ptr(), buffer, length);
    if (!r_bytes)
    {
        return -1;
    } else
    return 0;
}

dbust::SslFlavour& dbust::SslFlavour::instance()
{
    static SslFlavour flavour;
    return flavour;
}

int dbust::SslFlavour::close(dbust::Unit& unit) const
{
    ::SSL_shutdown(unit.get_ssl_ptr());
    ::SSL_clear(unit.get_ssl_ptr());
    ::close(unit.get_file_descriptor());
    return 0;
}

