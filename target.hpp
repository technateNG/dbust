#pragma once

#include <string>

class Target
{
    std::string host;

    std::string port;

    std::string resource_path;

    bool ssl{ false };

public:
    explicit Target(const char* url);
};