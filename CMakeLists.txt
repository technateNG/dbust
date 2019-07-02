cmake_minimum_required(VERSION 3.10.2)

project(dbust LANGUAGES CXX VERSION 0.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(MAKE_TESTS "Tests compilation." OFF)

add_subdirectory(src)

if (MAKE_TESTS)
    enable_testing()
    add_subdirectory(test)
    add_test(NAME tests COMMAND tests)
endif ()
