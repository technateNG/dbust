#include "response.hpp"

dbust::Response::Response(const std::string_view& raw) : str{ raw }
{
}

const char* const dbust::Response::get_status_code_ptr()
{
    return str.data() + 9;
}
