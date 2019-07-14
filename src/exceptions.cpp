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
