#pragma once

#include <iostream>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "configuration.hpp"

namespace dbust::models {
    class CmdParser {
        static const std::string description;

        static std::vector<std::string> parse_delimited_opt(const char *opt);

        static std::vector<std::string> load_dictionary(const char *file_name);

    public:
        Configuration &parse(int argc, char *argv[]);
    };
}