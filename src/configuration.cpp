#include <utility>

#include <utility>

#include "configuration.hpp"

dbust::models::Configuration dbust::models::Configuration::Builder::config(
        StatusCodes{"200", "201", "204", "301", "302", "307", "400", "401", "403", "500"},
        700,
        10
        );

const dbust::models::StatusCodes& dbust::models::Configuration::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& dbust::models::Configuration::get_file_extensions() const
{
    return file_extensions;
}

std::size_t dbust::models::Configuration::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

std::size_t dbust::models::Configuration::get_timeout() const
{
    return timeout;
}

const dbust::models::Target& dbust::models::Configuration::get_target() const
{
    return *target;
}

bool dbust::models::Configuration::is_get() const
{
    return get;
}

std::vector<std::string>& dbust::models::Configuration::get_dictionary()
{
    return dictionary;
}

const std::string& dbust::models::Configuration::get_user_agent() const
{
    return user_agent;
}

dbust::models::Configuration::Configuration(
        dbust::models::StatusCodes  sc,
        std::size_t nbs,
        std::size_t timeout
) : status_codes{std::move(sc)}, nb_of_sockets{nbs}, timeout{timeout}
{
}

dbust::models::Configuration::Builder::Builder() = default;

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_timeout(
        std::size_t new_timeout
)
{
    config.timeout = new_timeout;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_nb_of_sockets(
        std::size_t new_nb_of_sockets
)
{
    config.nb_of_sockets = new_nb_of_sockets;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_status_codes(
        const StatusCodes& new_status_codes
)
{
    config.status_codes = new_status_codes;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_file_extensions(
        const std::vector<std::string>& new_file_extensions
)
{
    config.file_extensions = new_file_extensions;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_dictionary(
        const std::vector<std::string>& new_dictionary
)
{
    config.dictionary = new_dictionary;
    return *this;
}

dbust::models::Configuration& dbust::models::Configuration::Builder::build()
{
    return config;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_target(
        const Target& new_target
)
{
    config.target = &new_target;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_get(
        bool new_get
)
{
    config.get = new_get;
    return *this;
}

dbust::models::Configuration::Builder& dbust::models::Configuration::Builder::set_user_agent(
        std::string new_ua
)
{
    config.user_agent = std::move(new_ua);
    return *this;
}


