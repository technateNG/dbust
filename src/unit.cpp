#include "unit.hpp"

dbust::Unit::Unit(
        const ConnectionFlavour& flavour,
        pollfd& poll
) : flavour{ flavour }, poll{ poll }
{
}

void dbust::Unit::connect(const ::addrinfo& addrinfo)
{
    int res = flavour.connect(*this, addrinfo);
    if (res == -1)
    {
        std::cerr << "[!] Can't connect to target. Error message: " <<
                  strerror(errno) << ". Reconnect issued." << std::endl;
        state = dbust::Unit::State::DISCONNECTED_WITH_ERROR;
    }
    else if (res == -2)
    {
        std::cerr << "[!] Can't establish SSL connection to target. Reconnect issued." << std::endl;
        state = dbust::Unit::State::DISCONNECTED_WITH_ERROR;
    }
    else if (res == -3)
    {
        state = dbust::Unit::State::DISCONNECTED;
    }
}

void dbust::Unit::receive(char* buffer, std::size_t length)
{
    int res = flavour.receive(*this, buffer, length);
    if (res == -1)
    {
        std::cerr << "[!] Can't recv data from: " << path <<
                  ". Reconnect issued." << std::endl;
        state = dbust::Unit::State::BROKEN;
    }
    else
    {
        state = dbust::Unit::State::EMPTY;
    }
}

void dbust::Unit::send(std::string_view& request)
{
    int res = flavour.send(*this, request);
    if (res == -1)
    {
        std::cerr << "[!] Can't send data to: " << path <<
                  ". Reconnect issued." << std::endl;
        state = dbust::Unit::State::BROKEN;
    }
    else
    {
        state = dbust::Unit::State::SENDED;
    }
}

void dbust::Unit::prepare()
{
    flavour.prepare(*this);
}

void dbust::Unit::set_ssl_ptr(::ssl_st* ssl)
{
    ssl_ptr = ssl;
}

::ssl_st* dbust::Unit::get_ssl_ptr()
{
    return ssl_ptr;
}

void dbust::Unit::close()
{
    flavour.close(*this);
}

void dbust::Unit::set_path(std::string& new_path)
{
    path = new_path;
}

std::string& dbust::Unit::get_path()
{
    return path;
}

const std::chrono::time_point<std::chrono::system_clock>& dbust::Unit::get_timeout_tp() const
{
    return timeout_tp;
}

void dbust::Unit::set_timeout_tp(
        const std::chrono::time_point<std::chrono::system_clock>& time_point
)
{
    timeout_tp = time_point;
}

dbust::Unit::State dbust::Unit::get_state() const
{
    return state;
}

void dbust::Unit::set_state(dbust::Unit::State new_state)
{
    state = new_state;
}

const std::chrono::time_point<std::chrono::system_clock>& dbust::Unit::get_delay_tp() const
{
    return delay_tp;
}

void dbust::Unit::set_delay_tp(const std::chrono::time_point<std::chrono::system_clock>& delay_tp)
{
    Unit::delay_tp = delay_tp;
}

void dbust::Unit::set_file_descriptor(int fd)
{
    poll.fd = fd;
}

void dbust::Unit::set_poll_event_flags(unsigned short event_flags)
{
    poll.events = event_flags;
}

unsigned short dbust::Unit::get_poll_revents()
{
    return poll.revents;
}

int dbust::Unit::get_file_descriptor()
{
    return poll.fd;
}
