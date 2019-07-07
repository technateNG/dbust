#pragma once

#include <string>
#include <unistd.h>
#include <netdb.h>

namespace dbust::models
{
    struct Unit;
}

namespace dbust::flavours
{
    struct ConnectionFlavour
    {
        virtual int send(dbust::models::Unit& unit, const std::string& request) = 0;

        virtual int receive(dbust::models::Unit& unit, char* buffer, std::size_t length) = 0;

        virtual int connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) = 0;

        virtual int close(dbust::models::Unit& unit) = 0;

        virtual int prepare(dbust::models::Unit& unit) = 0;
    };
}