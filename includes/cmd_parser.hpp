#pragma once

#include <iostream>
#include <getopt.h>
#include "config.hpp"
#include "dictionary_reader.hpp"
#include "batch_opt_parser.hpp"
#include "exceptions.hpp"

namespace dbust {
    class CmdParser {
        static const std::string description;

        const DictionaryReader& reader;

        const BatchOptParser& parser;

    public:
        CmdParser(const DictionaryReader& d_reader, const BatchOptParser& parser);

        Config parse(int argc, const char* argv[]);
    };
}