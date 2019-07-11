#include "target.hpp"
#include "config.hpp"

const dbust::StatusCodes& dbust::Config::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& dbust::Config::get_file_extensions() const
{
    return file_extensions;
}

std::size_t dbust::Config::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

std::size_t dbust::Config::get_timeout() const
{
    return timeout;
}

const dbust::Target& dbust::Config::get_target() const
{
    return target;
}

bool dbust::Config::is_get() const
{
    return get;
}

std::vector<std::string>& dbust::Config::get_dictionary()
{
    return dictionary;
}

const std::string& dbust::Config::get_user_agent() const
{
    return user_agent;
}

void dbust::Config::set_status_codes(const dbust::StatusCodes& new_status_codes)
{
    Config::status_codes = new_status_codes;
}

void dbust::Config::set_file_extensions(const std::vector<std::string>& new_file_extensions)
{
    Config::file_extensions = new_file_extensions;
}

void dbust::Config::set_dictionary(const std::vector<std::string>& new_dictionary)
{
    Config::dictionary = new_dictionary;
}

void dbust::Config::set_target(const dbust::Target& new_target)
{
    Config::target = new_target;
}

void dbust::Config::set_nb_of_sockets(size_t new_nb_of_sockets)
{
    Config::nb_of_sockets = new_nb_of_sockets;
}

void dbust::Config::set_timeout(size_t new_timeout)
{
    Config::timeout = new_timeout;
}

void dbust::Config::set_get(bool new_get)
{
    Config::get = new_get;
}

void dbust::Config::set_user_agent(const std::string& new_user_agent)
{
    Config::user_agent = new_user_agent;
}

size_t dbust::Config::get_delay() const
{
    return delay;
}

void dbust::Config::set_delay(size_t delay)
{
    Config::delay = delay;
}

dbust::Config::Config() = default;




