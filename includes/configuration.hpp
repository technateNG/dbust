#pragma once

#include <unordered_set>
#include <vector>
#include "target.hpp"
namespace dbust::models {
    class Configuration {
        std::unordered_set<std::string> status_codes;

        std::vector<std::string> file_extensions{""};

        std::vector<std::string> dictionary;

        const Target *target{nullptr};

        std::size_t nb_of_sockets{0};

        std::size_t timeout{0};

        bool get{false};

        std::string user_agent{"dbust"};

        Configuration();

    public:
        size_t get_timeout() const;

        const std::string &get_user_agent() const;

        const std::unordered_set<std::string> &get_status_codes() const;

        const std::vector<std::string> &get_file_extensions() const;

        std::size_t get_nb_of_sockets() const;

        const Target &get_target() const;

        std::vector<std::string> &get_dictionary();

        bool is_get() const;

        class Builder {
            static Configuration config;

        public:
            Builder &set_dictionary(const std::vector<std::string> &new_dictionary);

            Builder &set_file_extensions(const std::vector<std::string> &new_file_extensions);

            Builder &set_status_codes(const std::unordered_set<std::string> &new_status_codes);

            Builder &set_nb_of_sockets(std::size_t new_nb_of_sockets);

            Builder &set_timeout(std::size_t new_timeout);

            Builder &set_target(const Target &new_target);

            Builder &set_get(bool new_get);

            Builder &set_user_agent(std::string new_agent);

            Configuration &build();

            Builder();
        };
    };
}