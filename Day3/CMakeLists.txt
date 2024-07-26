cmake_minimum_required(VERSION 3.16)
project(step)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-fno-inline-functions)

add_executable(step src/main.cpp 
               src/cmd/ArgumentParser.cpp src/cmd/ArgumentParser.hpp
               src/IReader.hpp src/IReader.cpp
               src/FileReader.cpp src/FileReader.hpp
               src/LineReader.cpp src/LineReader.hpp
               src/LexemeReader.cpp src/LexemeReader.hpp
               src/Lexeme.cpp src/Lexeme.hpp
               src/IError.cpp src/IError.hpp
               src/ErrorManager.cpp src/ErrorManager.hpp
               src/PreCompilationError.cpp src/PreCompilationError.hpp
               src/LexicalError.cpp src/LexicalError.hpp
               src/UnrecognizedToken.cpp src/UnrecognizedToken.hpp
               src/NumberTooLarge.cpp src/NumberTooLarge.hpp
               src/NumberTooSmall.cpp src/NumberTooSmall.hpp
               src/UnexpectedEoi.cpp src/UnexpectedEoi.hpp
               src/MisplacedNumSeparator.cpp src/MisplacedNumSeparator.hpp
               src/ExpectationError.cpp src/ExpectationError.hpp
)
