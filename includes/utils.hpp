#pragma once

#include "config.hpp"
#include <fcntl.h>
#include <netdb.h>
#include <string>

namespace dbust {
int get_socket(const addrinfo& addrinfo);

std::string create_request(const std::string_view& path, const dbust::Config& config);

template <typename T>
void fill_buffer(T* buff, std::size_t& buff_idx)
{
}

template <typename T, typename E, typename... Args>
void fill_buffer(T* buff, std::size_t& buff_idx, E& tar, Args... args)
{
    for (auto& elem : tar) {
        buff[buff_idx] = elem;
        ++buff_idx;
    }
    return fill_buffer(buff, buff_idx, args...);
}
}