#pragma once

#include <netdb.h>
#include <string>
#include "configuration.hpp"

namespace dbust::utils
{
    int get_socket(addrinfo& addrinfo);

    std::string create_request(std::string& path, dbust::models::Configuration& config);

    bool is_in_status_codes(const char* buff, const std::unordered_set<std::string>& status_codes);
}