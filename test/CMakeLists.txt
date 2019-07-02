cmake_minimum_required(VERSION 3.10.2)

find_package(Catch2 REQUIRED)

set(TEST_SOURCES 
	sc_and_resp.cpp;
	cmd_parser.cpp;
	unit.cpp;
	batch_opt_parser.cpp;
	target.cpp)

add_executable(dbust-tests ${TEST_SOURCES})
target_compile_features(dbust-tests PRIVATE cxx_std_17)
target_link_libraries(dbust-tests dbust Catch2::Catch2)
