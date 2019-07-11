#pragma once

#include "connection_flavour.hpp"

namespace dbust
{
    struct HttpFlavour : public ConnectionFlavour
    {
        ~HttpFlavour();

        HttpFlavour(const HttpFlavour& flavour) = delete;

        HttpFlavour(HttpFlavour&& flavour) noexcept = delete;

        HttpFlavour& operator=(const HttpFlavour& flavour) = delete;

        HttpFlavour& operator=(HttpFlavour&& flavour) noexcept = delete;

        int send(dbust::Unit& unit, const std::string& request) override;

        int receive(dbust::Unit& unit, char* buffer, std::size_t length) override;

        int connect(dbust::Unit& unit, const ::addrinfo& addrinfo) override;

        int prepare(dbust::Unit& unit) override;

        int close(dbust::Unit& unit) override;

        static HttpFlavour& instance();

    private:
        explicit HttpFlavour();
    };
}