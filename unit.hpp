#pragma once

#include <chrono>
#include "ssl_flavour.hpp"

struct Unit {
    enum class State
    {
        EXCEPTION,
        EMPTY,
        SENDED
    };

    void send(std::string& request);

    void receive(char* buffer, std::size_t length);

    void connect(const ::addrinfo& addrinfo);

    void prepare();

    void close();

    void set_ssl_ptr(::ssl_st *ssl);

    ssl_st* get_ssl_ptr();

    int get_file_descriptor();

    void set_file_descriptor(int fd);

    void set_path(std::string& new_path);

    std::string& get_path();

    const std::chrono::time_point<std::chrono::system_clock> &get_time_point() const;

    void set_time_point(
            const std::chrono::time_point<std::chrono::system_clock> &time_point
            );

    void set_state(State state);

    State get_state() const;

    explicit Unit(ConnectionFlavour& flavour, int fd);
private:
    ConnectionFlavour& flavour;
    ::ssl_st* ssl_ptr{ nullptr };
    State state{ State::EMPTY };
    int file_descriptor;
    std::string path;
    std::chrono::time_point<std::chrono::system_clock> time_point;
};