#pragma once

#include <string>
#include <unistd.h>
#include <netdb.h>

struct Unit;
struct ConnectionFlavour
{
    virtual void send(Unit& unit, std::string& request) = 0;
    virtual void receive(Unit& unit, char* buffer, std::size_t length) = 0;
    virtual void connect(Unit& unit, const ::addrinfo& addrinfo) = 0;
    virtual void close(Unit& unit) = 0;
    virtual void prepare(Unit& unit) = 0;
};