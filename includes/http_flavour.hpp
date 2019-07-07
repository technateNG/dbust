#pragma once

#include "connection_flavour.hpp"

namespace dbust::flavours
{
    struct HttpFlavour : public ConnectionFlavour
    {
        ~HttpFlavour();

        HttpFlavour(const HttpFlavour& flavour) = delete;

        HttpFlavour(HttpFlavour&& flavour) noexcept = delete;

        HttpFlavour& operator=(const HttpFlavour& flavour) = delete;

        HttpFlavour& operator=(HttpFlavour&& flavour) noexcept = delete;

        int send(dbust::models::Unit& unit, const std::string& request) override;

        int receive(dbust::models::Unit& unit, char* buffer, std::size_t length) override;

        int connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) override;

        int prepare(dbust::models::Unit& unit) override;

        int close(dbust::models::Unit& unit) override;

        static HttpFlavour& instance();

    private:
        explicit HttpFlavour();
    };
}