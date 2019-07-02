#pragma once

#include <unordered_set>
#include <vector>
#include "target.hpp"
#include "status_codes.hpp"

namespace dbust
{
    class Config
    {
        StatusCodes status_codes{ "200", "201", "204", "301", "302", "307", "400", "401", "403", "500" };

        std::vector<std::string> file_extensions{ "" };

        std::vector<std::string> dictionary;

        Target target;

        std::size_t nb_of_sockets{ 100 };

        std::size_t timeout{ 10 };

        bool get{ false };

        std::string user_agent{ "dbust" };

        std::size_t delay{ 100 };

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
}