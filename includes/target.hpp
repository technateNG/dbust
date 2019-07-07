#pragma once

#include <string>

namespace dbust::models
{
    class Target
    {
        std::string host;

        std::string port;

        std::string resource_path;

        bool ssl{false};

    public:
        const std::string& get_host() const;

        const std::string& get_port() const;

        const std::string& get_resource_path() const;

        bool is_ssl() const;

        explicit Target(const char* url);
    };
}