#include <utility>

#include "unit.hpp"

Unit::Unit(ConnectionFlavour &flavour, int fd) : flavour{ flavour }, file_descriptor{ fd }
{
}

void Unit::connect(const ::addrinfo &addrinfo)
{
    flavour.connect(*this, addrinfo);
}

void Unit::receive(char* buffer, std::size_t length)
{
    flavour.receive(*this, buffer, length);
}

void Unit::send(std::string &request)
{
    flavour.send(*this, request);
    state = Unit::State::SENDED;
}

void Unit::prepare()
{
    flavour.prepare(*this);
    state = Unit::State::EMPTY;
}

void Unit::set_ssl_ptr(::ssl_st *ssl)
{
    ssl_ptr = ssl;
}

int Unit::get_file_descriptor()
{
    return file_descriptor;
}

ssl_st* Unit::get_ssl_ptr()
{
    return ssl_ptr;
}

void Unit::close()
{
    flavour.close(*this);
}

void Unit::set_file_descriptor(int fd)
{
    file_descriptor = fd;
}

void Unit::set_path(std::string& new_path)
{
    path = new_path;
}

std::string& Unit::get_path()
{
    return path;
}

const std::chrono::time_point<std::chrono::system_clock>& Unit::get_time_point() const
{
    return time_point;
}

void Unit::set_time_point(
        const std::chrono::time_point<std::chrono::system_clock> &new_time_point
        )
{
    time_point = new_time_point;
}

Unit::State Unit::get_state() const
{
    return state;
}

void Unit::set_state(Unit::State new_state)
{
    Unit::state = new_state;
}

