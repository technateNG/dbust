#include "status_codes.hpp"

dbust::StatusCodes::StatusCodes(std::initializer_list<std::string> scs)
    : raw_sc { std::unordered_set<std::string> { scs } }
{
}

bool dbust::StatusCodes::contains(const std::string_view& str) const
{
    return raw_sc.count(str.data());
}

bool dbust::StatusCodes::operator==(const dbust::StatusCodes& rhs) const
{
    return raw_sc == rhs.raw_sc;
}

bool dbust::StatusCodes::operator!=(const dbust::StatusCodes& rhs) const
{
    return !(rhs == *this);
}
