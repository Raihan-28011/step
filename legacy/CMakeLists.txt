cmake_minimum_required(VERSION 3.16)
project(step)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-fno-inline-functions)

add_executable(step src/main.cpp src/common.h src/token.h src/lexer.cpp src/lexer.h src/error.h src/main.h src/ast.cpp src/ast.h src/parser.cpp src/parser.h src/token.cpp src/object.cpp src/object.h src/frame.cpp src/frame.h src/error.cpp)
