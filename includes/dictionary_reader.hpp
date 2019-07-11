#pragma once

#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>

namespace dbust
{
    struct DictionaryReader
    {
        virtual std::vector<std::string> read_dictionary(const char* path) const = 0;
    };

    struct MMapDictionaryReader : public DictionaryReader
    {
        std::vector<std::string> read_dictionary(const char* path) const override;
    };
}

