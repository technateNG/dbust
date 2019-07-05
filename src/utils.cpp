#include "utils.hpp"

int dbust::utils::get_socket(addrinfo& addrinfo)
{
    return ::socket(addrinfo.ai_family, addrinfo.ai_socktype, addrinfo.ai_protocol);
}

std::string dbust::utils::create_request(std::string& path, dbust::models::Configuration& config)
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

bool dbust::utils::is_in_status_codes(
        const char* buff,
        const std::unordered_set<std::string>& status_codes)
{
    std::string_view buff_view(buff);
    std::string_view sc{ buff_view.substr(9, 3) };
    const char* scd = sc.cbegin();
    int res = status_codes.count(scd);
    return res;
}