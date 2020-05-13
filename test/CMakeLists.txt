cmake_minimum_required(VERSION 3.10.2)

find_package(Catch2 REQUIRED)

set(TEST_SOURCES config.cpp) 

add_executable(dbust-tests ${TEST_SOURCES})
target_compile_features(dbust-tests PUBLIC cxx_std_17)
target_link_libraries(dbust-tests dbust Catch2::Catch2)
