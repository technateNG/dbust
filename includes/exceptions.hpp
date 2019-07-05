#pragma once

#include <string>
#include <exception>

struct IncorrectUrl : public std::exception
{
    const char* what() const noexcept override ;
};

struct ArgumentNotSet : public std::exception
{
    const char *what() const noexcept override;

    explicit ArgumentNotSet(std::string arg_name);
private:
    std::string arg_name;
};

struct UnexpectedException : public std::exception
{
    const char *what() const noexcept override;
};