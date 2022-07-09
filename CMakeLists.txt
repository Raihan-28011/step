cmake_minimum_required(VERSION 3.16)
project(step)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

add_executable(step src/main.cpp src/common.h src/token.h src/lexer.cpp src/lexer.h src/error.h src/main.h)
