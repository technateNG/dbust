cmake_minimum_required(VERSION 3.14)

project(dbust)

include_directories(includes)

add_subdirectory(src)
add_subdirectory(test)

#enable_testing()
#add_test(NAME test COMMAND test)