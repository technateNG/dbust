#pragma once

#include <string>

namespace dbust
{
    class Response
    {
        std::string str;
    public:
        explicit Response(const char* raw);
        const char* get_status_code_ptr();
    };
}