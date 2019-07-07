cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)

add_executable(tests test.cpp cmd_parser.cpp)

target_link_libraries(tests libdbust Catch2::Catch2)