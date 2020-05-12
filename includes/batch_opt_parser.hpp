#pragma once

#include <string>
#include <vector>

namespace dbust {
struct BatchOptParser {
    std::vector<std::string> parse(const std::string_view& opt) const;
};
}