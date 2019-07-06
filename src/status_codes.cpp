#include "status_codes.hpp"

dbust::models::StatusCodes::StatusCodes(std::initializer_list<std::string> scs) :
        raw_sc{ std::unordered_set<std::string>{scs} }
{
}

bool dbust::models::StatusCodes::contains(const char* str) const
{
    return raw_sc.count(str);
}