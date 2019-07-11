#include "response.hpp"

dbust::Response::Response(const char* raw) : str{ raw }
{
}

const char* dbust::Response::get_status_code_ptr()
{
    return str.data() + 9;
}
