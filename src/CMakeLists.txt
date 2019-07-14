cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL)

add_executable(dbust
        main.cpp
        ssl_flavour.cpp
        http_flavour.cpp
        unit.cpp
        config.cpp
        cmd_parser.cpp
        target.cpp
        exceptions.cpp
        utils.cpp
        status_codes.cpp
        response.cpp
        dictionary_reader.cpp
        batch_opt_parser.cpp)

if(DEV)
add_library(libdbust
        ssl_flavour.cpp
        http_flavour.cpp
        unit.cpp
        config.cpp
        cmd_parser.cpp
        target.cpp
        exceptions.cpp
        utils.cpp
        status_codes.cpp
        response.cpp
        dictionary_reader.cpp
        batch_opt_parser.cpp)
endif()

target_link_libraries(dbust OpenSSL::SSL)