#pragma once

#include "connection_flavour.hpp"

namespace dbust {
struct HttpFlavour : public ConnectionFlavour {
    ~HttpFlavour();

    HttpFlavour(const HttpFlavour& flavour) = delete;

    HttpFlavour(HttpFlavour&& flavour) noexcept = delete;

    HttpFlavour& operator=(const HttpFlavour& flavour) = delete;

    HttpFlavour& operator=(HttpFlavour&& flavour) noexcept = delete;

    int send(dbust::Unit& unit, const std::string_view& request) const override;

    int receive(dbust::Unit& unit, char* buffer, std::size_t length) const override;

    int connect(dbust::Unit& unit, const ::addrinfo& addrinfo) const override;

    int prepare(dbust::Unit& unit) const override;

    int close(dbust::Unit& unit) const override;

    static HttpFlavour& instance();

private:
    explicit HttpFlavour();
};
}