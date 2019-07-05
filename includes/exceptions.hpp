#pragma once

#include <string>
#include <exception>

namespace dbust::exceptions {
    struct IncorrectUrlException : public std::exception {
        const char *what() const noexcept override;
    };

    struct ArgumentNotSetException : public std::exception {
        const char *what() const noexcept override;

        explicit ArgumentNotSetException(std::string arg_name);

    private:
        std::string arg_name;
    };

    struct UnexpectedException : public std::exception {
        const char *what() const noexcept override;
    };
}