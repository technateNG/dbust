#pragma once

#include <vector>
#include <string>

namespace dbust::models
{
    struct BatchOptParser
    {
        std::vector<std::string> parse(const char* opt) const;
    };
}