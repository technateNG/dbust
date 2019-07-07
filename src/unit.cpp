#include <utility>

#include "unit.hpp"

dbust::models::Unit::Unit(
        dbust::flavours::ConnectionFlavour& flavour,
        int fd
) : flavour{flavour}, file_descriptor{fd}
{
}

void dbust::models::Unit::connect(const ::addrinfo& addrinfo)
{
    int res = flavour.connect(*this, addrinfo);
    if (res == -1)
    {
        std::cerr << "[!] Can't connect to target. Error message: " <<
                  strerror(errno) << ". Reconnect issued." << std::endl;
        state = dbust::models::Unit::State::DICONNECTED;
    }
    else if (res == -2)
    {
        std::cerr << "[!] Can't establish SSL connection to target. Reconnect issued." << std::endl;
        state = dbust::models::Unit::State::DICONNECTED;
    }
}

void dbust::models::Unit::receive(char* buffer, std::size_t length)
{
    int res = flavour.receive(*this, buffer, length);
    if (res == -1)
    {
        std::cerr << "[!] Can't recv data from: " << path <<
                  ". Reconnect issued." << std::endl;
        state = dbust::models::Unit::State::BROKEN;
    }
    else
    {
        state = dbust::models::Unit::State::EMPTY;
    }
}

void dbust::models::Unit::send(std::string& request)
{
    int res = flavour.send(*this, request);
    if (res == -1)
    {
        std::cerr << "[!] Can't send data to: " << path <<
                  ". Reconnect issued." << std::endl;
        state = dbust::models::Unit::State::BROKEN;
    }
    else
    {
        state = dbust::models::Unit::State::SENDED;
    }
}

void dbust::models::Unit::prepare()
{
    flavour.prepare(*this);
}

void dbust::models::Unit::set_ssl_ptr(::ssl_st* ssl)
{
    ssl_ptr = ssl;
}

int dbust::models::Unit::get_file_descriptor()
{
    return file_descriptor;
}

::ssl_st* dbust::models::Unit::get_ssl_ptr()
{
    return ssl_ptr;
}

void dbust::models::Unit::close()
{
    flavour.close(*this);
}

void dbust::models::Unit::set_file_descriptor(int fd)
{
    file_descriptor = fd;
}

void dbust::models::Unit::set_path(std::string& new_path)
{
    path = new_path;
}

std::string& dbust::models::Unit::get_path()
{
    return path;
}

const std::chrono::time_point<std::chrono::system_clock>& dbust::models::Unit::get_time_point() const
{
    return time_point;
}

void dbust::models::Unit::set_time_point(
        const std::chrono::time_point<std::chrono::system_clock>& new_time_point
)
{
    time_point = new_time_point;
}

dbust::models::Unit::State dbust::models::Unit::get_state() const
{
    return state;
}

void dbust::models::Unit::set_state(dbust::models::Unit::State new_state)
{
    state = new_state;
}


