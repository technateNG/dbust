cmake_minimum_required(VERSION 3.10.2)

set(SOURCES config.cpp exceptions.cpp utils.cpp main.cpp)

find_package(OpenSSL REQUIRED)

add_library(dbust STATIC ${SOURCES})
target_compile_features(dbust PUBLIC cxx_std_17)
target_include_directories(dbust PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/includes>)

add_executable(dbust-bin main.cpp)
target_compile_features(dbust-bin PRIVATE cxx_std_17)
target_link_libraries(dbust-bin PRIVATE dbust PUBLIC OpenSSL::SSL)
set_target_properties(dbust-bin PROPERTIES OUTPUT_NAME "dbust")
