cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL)

add_executable(dbust
        main.cpp
        ssl_flavour.cpp
        http_flavour.cpp
        unit.cpp
        configuration.cpp
        cmd_parser.cpp
        target.cpp
        exceptions.cpp)

target_link_libraries(dbust OpenSSL::SSL)