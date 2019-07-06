#include "response.hpp"

dbust::models::Response::Response(const char* raw) : str{ raw }
{
}

const char* dbust::models::Response::get_status_code_ptr()
{
    return str.data() + 9;
}
