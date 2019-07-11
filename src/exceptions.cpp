#include <utility>

#include "exceptions.hpp"

const char* dbust::IncorrectUrlException::what() const noexcept
{
    return "[!] Provided URL is incorrect.";
}

const char* dbust::ArgumentNotSetException::what() const noexcept
{
    static std::string msg{ "[!] Required argument not set: " };
    msg += arg_name;
    msg += '.';
    return msg.data();
}

dbust::ArgumentNotSetException::ArgumentNotSetException(std::string arg_name)
: arg_name{ std::move(arg_name) }
{
}

const char* dbust::UnexpectedException::what() const noexcept
{
    return "[!] Something unexpected happen. Please report this.";
}
