#pragma once

#include <iostream>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "connection_flavour.hpp"

struct SslFlavour : public ConnectionFlavour
{
    ~SslFlavour() = default;

    SslFlavour(const SslFlavour& flavour) = delete;

    SslFlavour(SslFlavour&& flvour) = delete;

    SslFlavour& operator=(const SslFlavour& flavour) = delete;

    SslFlavour& operator=(SslFlavour&& flavour) = delete;

    void connect(Unit& unit, const ::addrinfo& addrinfo) override;

    void send(Unit& unit, std::string& request) override;

    void receive(Unit& unit, char* buffer, std::size_t length) override;

    void prepare(Unit& unit) override;

    void close(Unit &unit) override;

    static SslFlavour& instance(::SSL_CTX& ctx);
private:
    explicit SslFlavour(SSL_CTX& ctx);
    SSL_CTX& ctx;
};