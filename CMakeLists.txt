cmake_minimum_required(VERSION 3.17)
project(Regex_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(Regex_parser main.cpp tokenizer.hpp ast.hpp finite_automata.hpp)