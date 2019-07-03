cmake_minimum_required(VERSION 3.14)
project(dbust)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL)

add_executable(dbust main.cpp ssl_flavour.cpp http_flavour.cpp unit.cpp connection_flavour.hpp configuration.cpp cmd_parser.cpp target.cpp)

target_link_libraries(dbust OpenSSL::SSL)