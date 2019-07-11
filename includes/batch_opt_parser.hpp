#pragma once

#include <vector>
#include <string>

namespace dbust
{
    struct BatchOptParser
    {
        std::vector<std::string> parse(const char* opt) const;
    };
}