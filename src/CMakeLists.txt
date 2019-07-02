cmake_minimum_required(VERSION 3.10.2)

set(SOURCES 
	ssl_flavour.cpp;
	http_flavour.cpp;
	unit.cpp;config.cpp;
	cmd_parser.cpp;
	target.cpp;
	exceptions.cpp;
	utils.cpp;
	status_codes.cpp;
	response.cpp;
	dictionary_reader.cpp;
	batch_opt_parser.cpp;
	unit_pool.cpp)

find_package(OpenSSL REQUIRED)

add_library(dbust STATIC ${SOURCES})
target_compile_features(dbust PUBLIC cxx_std_17)
target_include_directories(dbust PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/includes>)

add_executable(dbust-bin main.cpp)
target_compile_features(dbust-bin PRIVATE cxx_std_17)
target_link_libraries(dbust-bin PRIVATE dbust PUBLIC OpenSSL::SSL)
set_target_properties(dbust-bin PROPERTIES OUTPUT_NAME "dbust")
