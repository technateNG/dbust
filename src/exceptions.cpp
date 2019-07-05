#include <utility>

#include "exceptions.hpp"

const char* dbust::exceptions::IncorrectUrlException::what() const noexcept
{
    return "[!] Provided URL is incorrect.";
}

const char* dbust::exceptions::ArgumentNotSetException::what() const noexcept
{
    static std::string msg{ "[!] Required argument not set: " };
    msg += arg_name;
    msg += '.';
    return msg.data();
}

dbust::exceptions::ArgumentNotSetException::ArgumentNotSetException(std::string arg_name)
: arg_name{ std::move(arg_name) }
{
}

const char* dbust::exceptions::UnexpectedException::what() const noexcept
{
    return "[!] Something unexpected happen. Please report this.";
}
