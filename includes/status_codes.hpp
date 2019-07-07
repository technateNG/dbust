#pragma once

#include <unordered_set>

namespace dbust::models
{
    class StatusCodes
    {
        std::unordered_set<std::string> raw_sc;

    public:
        StatusCodes(std::initializer_list<std::string> scs);

        template<typename InputIt>
        StatusCodes(InputIt first, InputIt last) : raw_sc{ std::unordered_set<std::string>(first, last) }
        {
        }

        bool contains(const char* str) const;

        bool operator==(const StatusCodes& rhs) const;

        bool operator!=(const StatusCodes& rhs) const;
    };
}