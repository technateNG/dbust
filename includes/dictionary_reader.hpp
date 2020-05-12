#pragma once

#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <vector>

namespace dbust {
struct DictionaryReader {
    virtual std::vector<std::string> read_dictionary(std::string_view path) const = 0;
};

struct MMapDictionaryReader : public DictionaryReader {
    std::vector<std::string> read_dictionary(std::string_view path) const override;
};
}
