#include "ssl_flavour.hpp"
#include "unit.hpp"

SslFlavour::SslFlavour(
        ::SSL_CTX& ctx
        ) : ctx{ ctx }
{
}

void SslFlavour::prepare(Unit& unit)
{
    unit.set_ssl_ptr(::SSL_new(&ctx));
    SSL_set_fd(unit.get_ssl_ptr(), unit.get_file_descriptor());
}

void SslFlavour::connect(Unit& unit,  const ::addrinfo& addrinfo)
{
    ::connect(unit.get_file_descriptor(), addrinfo.ai_addr, addrinfo.ai_addrlen);
    ::SSL_connect(unit.get_ssl_ptr());
}

void SslFlavour::send(Unit& unit, std::string& request)
{
    ::SSL_write(unit.get_ssl_ptr(), request.c_str(), request.length());
}

void SslFlavour::receive(Unit& unit, char* buffer, const std::size_t length)
{
    ::SSL_read(unit.get_ssl_ptr(), buffer, length);
}

SslFlavour &SslFlavour::instance(::SSL_CTX& ctx)
{
    static SslFlavour flavour(ctx);
    return flavour;
}

void SslFlavour::close(Unit &unit)
{
    ::SSL_shutdown(unit.get_ssl_ptr());
    ::SSL_clear(unit.get_ssl_ptr());
    ::SSL_free(unit.get_ssl_ptr());
}

