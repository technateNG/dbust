#pragma once

#include <string>
#include <unistd.h>
#include <netdb.h>

namespace dbust::models
{
    struct Unit;
}
namespace dbust::flavours {
    struct ConnectionFlavour {
        virtual void send(dbust::models::Unit& unit, std::string& request) = 0;

        virtual void receive(dbust::models::Unit& unit, char *buffer, std::size_t length) = 0;

        virtual void connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) = 0;

        virtual void close(dbust::models::Unit& unit) = 0;

        virtual void prepare(dbust::models::Unit& unit) = 0;
    };
}