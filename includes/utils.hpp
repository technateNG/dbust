#pragma once

#include <netdb.h>
#include <string>
#include "configuration.hpp"

namespace dbust::utils
{
    int get_socket(addrinfo& addrinfo);

    std::string create_request(std::string& path, dbust::models::Configuration& config);
}