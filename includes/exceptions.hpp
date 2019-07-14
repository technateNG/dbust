#pragma once

#include <string>
#include <exception>

namespace dbust {
    struct IncorrectUrlException : public std::exception {
        const char *what() const noexcept override;
    };

    struct UnexpectedException : public std::exception {
        const char *what() const noexcept override;
    };
}