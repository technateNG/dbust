#pragma once

#include <netdb.h>
#include <string>
#include <unistd.h>

namespace dbust {
struct Unit;
}

namespace dbust {
struct ConnectionFlavour {
    virtual int send(dbust::Unit& unit, const std::string_view& request) const = 0;

    virtual int receive(dbust::Unit& unit, char* buffer, std::size_t length) const = 0;

    virtual int connect(dbust::Unit& unit, const ::addrinfo& addrinfo) const = 0;

    virtual int close(dbust::Unit& unit) const = 0;

    virtual int prepare(dbust::Unit& unit) const = 0;
};
}