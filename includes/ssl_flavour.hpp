#pragma once

#include <iostream>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "connection_flavour.hpp"

namespace dbust::flavours
{
    struct SslFlavour : public ConnectionFlavour
    {
        ~SslFlavour() = default;

        SslFlavour(const SslFlavour& flavour) = delete;

        SslFlavour(SslFlavour&& flavour) = delete;

        SslFlavour& operator=(const SslFlavour& flavour) = delete;

        SslFlavour& operator=(SslFlavour&& flavour) = delete;

        void connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) override;

        void send(dbust::models::Unit& unit, std::string& request) override;

        void receive(dbust::models::Unit& unit, char* buffer, std::size_t length) override;

        void prepare(dbust::models::Unit& unit) override;

        void close(dbust::models::Unit& unit) override;

        static SslFlavour& instance();

    private:
        explicit SslFlavour();
    };
}