#include <utility>

#include <utility>

#include "configuration.hpp"

const std::unordered_set<std::string>& Configuration::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& Configuration::get_file_extensions() const
{
    return file_extensions;
}

const std::string& Configuration::get_host() const
{
    return host;
}

const std::string& Configuration::get_port() const
{
    return port;
}

bool Configuration::is_ssl() const
{
    return ssl;
}

std::size_t Configuration::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

Configuration::Configuration() = default;

Configuration::Builder::Builder() : config_ptr{ new Configuration() }
{
}

Configuration::Builder& Configuration::Builder::set_ssl(bool is_ssl)
{
    config_ptr->ssl = is_ssl;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_nb_of_sockets(std::size_t new_nb_of_sockets)
{
    config_ptr->nb_of_sockets = new_nb_of_sockets;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_status_codes(const std::unordered_set<std::string>& new_status_codes)
{
    config_ptr->status_codes = new_status_codes;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_file_extensions(const std::vector<std::string>& new_file_extensions)
{
    config_ptr->file_extensions = new_file_extensions;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_dictionary(const std::vector<std::string>& new_dictionary)
{
    config_ptr->dictionary = new_dictionary;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_port(const std::string& new_port)
{
    config_ptr->port = new_port;
    return *this;
}

Configuration::Builder& Configuration::Builder::set_host(const std::string& new_host)
{
    config_ptr->host = new_host;
    return *this;
}

Configuration& Configuration::Builder::build() {
    return *config_ptr;
}


