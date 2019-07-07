#pragma once

#include <netdb.h>
#include <string>
#include "config.hpp"

namespace dbust::utils
{
    int get_socket(addrinfo& addrinfo);

    std::string create_request(std::string& path, dbust::models::Config& config);
}