#include "status_codes.hpp"

dbust::models::StatusCodes::StatusCodes(std::initializer_list<std::string> scs) :
        raw_sc{ std::unordered_set<std::string>{scs} }
{
}

bool dbust::models::StatusCodes::contains(const char* str) const
{
    return raw_sc.count(str);
}

bool dbust::models::StatusCodes::operator==(const dbust::models::StatusCodes& rhs) const
{
    return raw_sc == rhs.raw_sc;
}

bool dbust::models::StatusCodes::operator!=(const dbust::models::StatusCodes& rhs) const
{
    return !(rhs == *this);
}
