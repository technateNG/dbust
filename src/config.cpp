#include "target.hpp"
#include "config.hpp"

const dbust::models::StatusCodes& dbust::models::Config::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& dbust::models::Config::get_file_extensions() const
{
    return file_extensions;
}

std::size_t dbust::models::Config::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

std::size_t dbust::models::Config::get_timeout() const
{
    return timeout;
}

const dbust::models::Target& dbust::models::Config::get_target() const
{
    return target;
}

bool dbust::models::Config::is_get() const
{
    return get;
}

std::vector<std::string>& dbust::models::Config::get_dictionary()
{
    return dictionary;
}

const std::string& dbust::models::Config::get_user_agent() const
{
    return user_agent;
}

void dbust::models::Config::set_status_codes(const dbust::models::StatusCodes& new_status_codes)
{
    Config::status_codes = new_status_codes;
}

void dbust::models::Config::set_file_extensions(const std::vector<std::string>& new_file_extensions)
{
    Config::file_extensions = new_file_extensions;
}

void dbust::models::Config::set_dictionary(const std::vector<std::string>& new_dictionary)
{
    Config::dictionary = new_dictionary;
}

void dbust::models::Config::set_target(const dbust::models::Target& new_target)
{
    Config::target = new_target;
}

void dbust::models::Config::set_nb_of_sockets(size_t new_nb_of_sockets)
{
    Config::nb_of_sockets = new_nb_of_sockets;
}

void dbust::models::Config::set_timeout(size_t new_timeout)
{
    Config::timeout = new_timeout;
}

void dbust::models::Config::set_get(bool new_get)
{
    Config::get = new_get;
}

void dbust::models::Config::set_user_agent(const std::string& new_user_agent)
{
    Config::user_agent = new_user_agent;
}

dbust::models::Config::Config() = default;




