cmake_minimum_required(VERSION 3.14)

find_package(Catch2 REQUIRED)

add_executable(tests test.cpp)

target_link_libraries(tests Catch2::Catch2)