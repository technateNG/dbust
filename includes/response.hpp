#pragma once

#include <string>

namespace dbust::models
{
    class Response
    {
        std::string str;
    public:
        explicit Response(const char* raw);
        const char* get_status_code_ptr();
    };
}