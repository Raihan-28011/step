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
               src/NoInputFile.cpp src/NoInputFile.hpp
               src/UnrecognizedOption.cpp src/UnrecognizedOption.hpp
               src/MissingArgumentValue.cpp src/MissingArgumentValue.hpp
               src/NotValidFile.cpp src/NotValidFile.hpp
               src/GenericError.cpp src/GenericError.hpp
               src/UnrecognizedToken.cpp src/UnrecognizedToken.hpp
)
