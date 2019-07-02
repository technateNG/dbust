#pragma once

#include <string>
#include <exception>

namespace dbust
{
    struct IncorrectUrlException : public std::exception
    {
        enum class Part
        {
            PROTOCOL,
            CONVENTION,
            PORT
        };

        const char* what() const noexcept override;

        const Part get_part() const;

        explicit IncorrectUrlException(Part part);

    private:
        const Part part;
    };

    struct ArgumentNotSetException : public std::exception
    {
        enum class Arg
        {
            DIRECTORY,
            URL,
            ALL
        };

        const char* what() const noexcept override;

        Arg get_arg() const;

        explicit ArgumentNotSetException(Arg arg);

    private:
        Arg arg;
    };

    struct UnexpectedException : public std::exception
    {
        const char* what() const noexcept override;
    };
}