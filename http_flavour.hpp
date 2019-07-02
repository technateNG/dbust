#pragma once

#include "connection_flavour.hpp"

struct HttpFlavour : public ConnectionFlavour
{
    ~HttpFlavour();

    HttpFlavour(const HttpFlavour& flavour) = delete;

    HttpFlavour(HttpFlavour&& flavour) noexcept = delete;

    HttpFlavour& operator=(const HttpFlavour& flavour) = delete;

    HttpFlavour& operator=(HttpFlavour&& flavour) noexcept = delete;

    void send(Unit &unit, std::string &request) override;

    void receive(Unit &unit, char *buffer, std::size_t length) override;

    void connect(Unit &unit, const ::addrinfo &addrinfo) override;

    void prepare(Unit &unit) override;

    void close(Unit &unit) override;

    static HttpFlavour& instance();
private:
    explicit HttpFlavour();
};