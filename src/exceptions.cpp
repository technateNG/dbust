#include <utility>

#include <utility>

#include "exceptions.hpp"

const char* dbust::IncorrectUrlException::what() const noexcept
{
    return "[!] Provided URL is incorrect.";
}

const char* dbust::UnexpectedException::what() const noexcept
{
    return "[!] Something unexpected happen. Please report this.";
}

const char* dbust::ArgumentNotSetException::what() const noexcept
{
    return arg.c_str();
}

dbust::ArgumentNotSetException::ArgumentNotSetException(std::string  arg) : arg{ std::move(arg) }
{
}
