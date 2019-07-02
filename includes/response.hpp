#pragma once

#include <string>

namespace dbust
{
    class Response
    {
        std::string str;
    public:
        explicit Response(const std::string_view& raw);

        const char* const get_status_code_ptr();
    };
}