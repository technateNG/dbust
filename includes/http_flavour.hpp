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

        void send(dbust::models::Unit& unit, std::string& request) override;

        void receive(dbust::models::Unit& unit, char* buffer, std::size_t length) override;

        void connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) override;

        void prepare(dbust::models::Unit& unit) override;

        void close(dbust::models::Unit& unit) override;

        static HttpFlavour& instance();

    private:
        explicit HttpFlavour();
    };
}