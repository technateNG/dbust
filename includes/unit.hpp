#pragma once

#include "ssl_flavour.hpp"
#include <chrono>
#include <poll.h>

namespace dbust {
struct Unit {
    enum class State {
        BROKEN,
        EMPTY,
        SENDED,
        DISCONNECTED_WITH_ERROR,
        DISCONNECTED
    };

    void send(std::string_view& request);

    void receive(char* buffer, std::size_t length);

    void connect(const ::addrinfo& addrinfo);

    void prepare();

    void close();

    void set_ssl_ptr(::ssl_st* ssl);

    void set_path(std::string& new_path);

    void set_timeout_tp(
        const std::chrono::time_point<std::chrono::system_clock>& time_point);

    void set_delay_tp(const std::chrono::time_point<std::chrono::system_clock>& delay_tp);

    void set_state(State new_state);

    void set_file_descriptor(int fd);

    void set_poll_event_flags(unsigned short event_flags);

    ::ssl_st* get_ssl_ptr();

    std::string& get_path();

    const std::chrono::time_point<std::chrono::system_clock>& get_timeout_tp() const;

    const std::chrono::time_point<std::chrono::system_clock>& get_delay_tp() const;

    State get_state() const;

    unsigned short get_poll_revents();

    int get_file_descriptor();

    Unit(const ConnectionFlavour& flavour, pollfd& poll);

private:
    const ConnectionFlavour& flavour;

    ::ssl_st* ssl_ptr { nullptr };

    State state { State::EMPTY };

    std::string path;

    std::chrono::time_point<std::chrono::system_clock> timeout_tp;

    std::chrono::time_point<std::chrono::system_clock> delay_tp;

    ::pollfd& poll;
};
}