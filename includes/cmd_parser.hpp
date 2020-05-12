#pragma once

#include "batch_opt_parser.hpp"
#include "config.hpp"
#include "dictionary_reader.hpp"
#include "exceptions.hpp"
#include <getopt.h>
#include <iostream>

namespace dbust {
class CmdParser {
    const DictionaryReader& reader;

    const BatchOptParser& parser;

public:
    static const std::string description;

    CmdParser(const DictionaryReader& d_reader, const BatchOptParser& parser);

    Config parse(int argc, const char* argv[]);
};
}