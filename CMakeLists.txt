cmake_minimum_required(VERSION 3.10.2)

project(dbust LANGUAGES CXX VERSION 0.1)

option(DEV "Enable development and tests compilation." OFF)

include_directories(includes)

add_subdirectory(src)

if(DEV)
    enable_testing()
    add_subdirectory(test)
    add_test(NAME tests COMMAND tests)
endif()