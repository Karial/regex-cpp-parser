cmake_minimum_required(VERSION 3.14)
project(Regex_parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(Regex_parser test.cpp tokenizer.hpp ast.hpp finite_automata.hpp util.hpp character_classes.hpp)
add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)
target_link_libraries(Regex_parser gtest gtest_main)