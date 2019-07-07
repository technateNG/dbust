#include "utils.hpp"

int dbust::utils::get_socket(addrinfo& addrinfo)
{
    return ::socket(addrinfo.ai_family, addrinfo.ai_socktype, addrinfo.ai_protocol);
}

std::string dbust::utils::create_request(std::string& path, dbust::models::Config& config)
{
    std::string res{ config.is_get() ? "GET " :  "HEAD " };
    res += config.get_target().get_resource_path();
    res += path;
    res += " HTTP/1.1\r\nHost: ";
    res += config.get_target().get_host();
    res += "\r\nUser-Agent: ";
    res += config.get_user_agent();
    res += "\r\nAccept: */*\r\n\r\n";
    return res;
}
