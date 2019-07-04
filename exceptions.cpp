#include <utility>

#include "exceptions.hpp"

const char* IncorrectUrl::what() const noexcept
{
    return "[!] Provided URL is incorrect.";
}

const char* ArgumentNotSet::what() const noexcept
{
    static std::string msg{ "[!] Required argument not set: " };
    msg += arg_name;
    msg += '.';
    return msg.data();
}

ArgumentNotSet::ArgumentNotSet(std::string arg_name) : arg_name{ std::move(arg_name) }
{
}

const char* UnexpectedException::what() const noexcept
{
    return "[!] Something unexpected happen. Please report this.";
}
