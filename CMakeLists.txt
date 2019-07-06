cmake_minimum_required(VERSION 3.14)

project(dbust LANGUAGES CXX VERSION 0.1)

include_directories(includes)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
add_test(NAME dbust_tests COMMAND test)