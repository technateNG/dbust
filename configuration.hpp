#pragma once

#include <vector>
#include <unordered_set>
#include <bits/unique_ptr.h>
#include <bits/shared_ptr.h>

class Configuration
{
  std::unordered_set<std::string> status_codes;

  std::vector<std::string> file_extensions;

  std::vector<std::string> dictionary;

  std::string host;

  std::string port;

  bool ssl{ false };

  std::size_t nb_of_sockets{ 0 };

  Configuration();

public:
    const std::unordered_set<std::string>& get_status_codes() const;

    const std::vector<std::string>& get_file_extensions() const;

    const std::string& get_host() const;

    const std::string& get_port() const;

    bool is_ssl() const;

    std::size_t get_nb_of_sockets() const;

    class Builder
    {
        Configuration* config_ptr;

    public:
        Builder& set_host(const std::string& new_host);

        Builder& set_port(const std::string& new_port);

        Builder& set_dictionary(const std::vector<std::string>& new_dictionary);

        Builder& set_file_extensions(const std::vector<std::string>& new_file_extensions);

        Builder& set_status_codes(const std::unordered_set<std::string>& new_status_codes);

        Builder& set_nb_of_sockets(std::size_t new_nb_of_sockets);

        Builder& set_ssl(bool is_ssl);

        Configuration& build();

        Builder();
    };
};