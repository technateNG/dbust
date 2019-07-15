#pragma once

#include <string>
#include <exception>

namespace dbust
{
    struct IncorrectUrlException : public std::exception
    {
        const char* what() const noexcept override;
    };

    struct ArgumentNotSetException : public std::exception
    {
        const char* what() const noexcept override;

        explicit ArgumentNotSetException(std::string  arg);

    private:
        std::string arg;
    };

    struct UnexpectedException : public std::exception
    {
        const char* what() const noexcept override;
    };
}