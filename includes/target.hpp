#pragma once

#include <string>

class Target
{
    std::string host;

    std::string port;

    std::string resource_path;

    bool ssl{ false };
public:
    const std::string &get_host() const;

    const std::string &get_port() const;

    const std::string &get_resource_path() const;

    bool is_ssl() const;

private:

    explicit Target(const char* url);
public:
    static const Target& instance(const char* url);
};