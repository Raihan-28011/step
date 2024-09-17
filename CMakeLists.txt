cmake_minimum_required(VERSION 3.16)
project(step)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-gdwarf-3)
add_executable(step src/config.h 
                    src/typedefs.h
                    src/cmdopts.h src/cmdopts.c
                    src/step.c)
