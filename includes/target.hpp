#pragma once

#include <string>

namespace dbust {
class Config;
class Target {
    friend class Config;

    std::string host;

    std::string port;

    std::string resource_path;

    bool ssl { false };

    Target();

public:
    const std::string& get_host() const;

    const std::string& get_port() const;

    const std::string& get_resource_path() const;

    bool is_ssl() const;

    static Target parse_url(const std::string_view& url);
};
}