#include <utility>

#include "configuration.hpp"

Configuration Configuration::Builder::config;

const std::unordered_set<std::string>& Configuration::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& Configuration::get_file_extensions() const
{
    return file_extensions;
}

std::size_t Configuration::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

std::size_t Configuration::get_timeout() const
{
    return timeout;
}

const Target& Configuration::get_target() const
{
    return *target;
}

bool Configuration::is_get() const
{
    return get;
}

std::vector<std::string>& Configuration::get_dictionary()
{
    return dictionary;
}

const std::string& Configuration::get_user_agent() const
{
    return user_agent;
}

Configuration::Configuration() = default;

Configuration::Builder::Builder()
{
    config.status_codes =
            std::unordered_set<std::string>{
        "200", "201", "204", "301", "302", "307", "400", "401", "403", "500"
    };
    config.nb_of_sockets = 700;
    config.timeout = 10;
}

Configuration::Builder& Configuration::Builder::set_timeout(std::size_t new_timeout)
{
    config.timeout = new_timeout;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_nb_of_sockets(std::size_t new_nb_of_sockets)
{
    config.nb_of_sockets = new_nb_of_sockets;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_status_codes(const std::unordered_set<std::string>& new_status_codes)
{
    config.status_codes = new_status_codes;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_file_extensions(const std::vector<std::string>& new_file_extensions)
{
    config.file_extensions = new_file_extensions;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_dictionary(const std::vector<std::string>& new_dictionary)
{
    config.dictionary = new_dictionary;
    return *this;
}

Configuration& Configuration::Builder::build()
{
    return config;
}

Configuration::Builder& Configuration::Builder::set_target(const Target& new_target)
{
    config.target = &new_target;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_get(bool new_get)
{
    config.get = new_get;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_user_agent(std::string new_ua)
{
    config.user_agent = std::move(new_ua);
    return *this;
}


