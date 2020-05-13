#pragma once

#include <getopt.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unordered_set>
#include <vector>

#include "exceptions.hpp"

namespace dbust {
struct BatchOptParser {
    std::vector<std::string> parse(const std::string_view& opt) const;
};

struct DictionaryReader {
    virtual std::vector<std::string> read_dictionary(std::string_view path) const = 0;
};

struct MMapDictionaryReader : public DictionaryReader {
    std::vector<std::string> read_dictionary(std::string_view path) const override;
};

using StatusCodes = std::vector<std::string>;

class Target {
    std::string host;

    std::string port;

    std::string resource_path;

    bool ssl { false };
public:
    const std::string& get_host() const;

    const std::string& get_port() const;

    const std::string& get_resource_path() const;

    bool is_ssl() const;

    static Target parse_url(const std::string_view& url);
    
    Target();
};

class Config {
    StatusCodes status_codes { "200", "201", "204", "301", "302", "307", "400", "401", "403", "500" };

    std::vector<std::string> file_extensions { "" };

    std::vector<std::string> dictionary;

    Target target;

    std::size_t nb_of_sockets { 100 };

    std::size_t timeout { 10 };

    bool get { false };

    std::string user_agent { "dbust" };

    std::size_t delay { 100 };

public:
    Config();

    std::size_t get_timeout() const;

    const std::string& get_user_agent() const;

    const StatusCodes& get_status_codes() const;

    const std::vector<std::string>& get_file_extensions() const;

    std::size_t get_nb_of_sockets() const;

    const Target& get_target() const;

    const std::vector<std::string>& get_dictionary() const;

    bool is_get() const;

    void set_status_codes(const StatusCodes& status_codes);

    void set_file_extensions(const std::vector<std::string>& file_extensions);

    void set_dictionary(const std::vector<std::string>& dictionary);

    void set_target(const Target& target);

    void set_nb_of_sockets(std::size_t nb_of_sockets);

    void set_timeout(std::size_t timeout);

    void set_get(bool get);

    void set_user_agent(const std::string& user_agent);

    std::size_t get_delay() const;

    void set_delay(std::size_t delay);
};

class CmdParser {
    const DictionaryReader& reader;

    const BatchOptParser& parser;

public:
    static const std::string description;

    CmdParser(const DictionaryReader& d_reader, const BatchOptParser& parser);

    Config parse(int argc, const char* argv[]);
};
}
