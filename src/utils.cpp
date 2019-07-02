#include "utils.hpp"

int dbust::get_socket(const addrinfo& addrinfo)
{
    return ::socket(
            addrinfo.ai_family,
            addrinfo.ai_socktype, // | SOCK_NONBLOCK,
            addrinfo.ai_protocol
    );
}

std::string dbust::create_request(const std::string_view& path, const dbust::Config& config)
{
    std::string res{ config.is_get() ? "GET " :  "HEAD " }; //4 | 5
    res += config.get_target().get_resource_path();
    res += path;
    res += " HTTP/1.1\r\nHost: "; // 19
    res += config.get_target().get_host();
    res += "\r\nUser-Agent: "; // 16
    res += config.get_user_agent();
    res += "\r\nAccept: */*\r\n\r\n"; // 23
    return res;
}