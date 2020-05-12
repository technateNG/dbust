#include "exceptions.hpp"

const char* dbust::IncorrectUrlException::what() const noexcept
{
    return "[!] Incorrect url.";
}

dbust::IncorrectUrlException::IncorrectUrlException(Part part)
    : part { part }
{
}

const dbust::IncorrectUrlException::Part dbust::IncorrectUrlException::get_part() const
{
    return part;
}

const char* dbust::UnexpectedException::what() const noexcept
{
    return "[!] Something unexpected happen. Please report this.";
}

const char* dbust::ArgumentNotSetException::what() const noexcept
{
    return "[!] Required argument not set.";
}

dbust::ArgumentNotSetException::ArgumentNotSetException(Arg arg)
    : arg { arg }
{
}

dbust::ArgumentNotSetException::Arg dbust::ArgumentNotSetException::get_arg() const
{
    return arg;
}
