cmake_minimum_required(VERSION 3.10)
project(tetrisrec)

set(CMAKE_VERBOSE_MAKEFILE on)
##set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra  -Wstrict-aliasing -pedantic -Werror -Wunreachable-code -Wcast-align -Wundef -Wno-unused")
set(CMAKE_CXX_STANDARD 11)

add_executable(tetrisrec main.cpp main.h treiber.cpp treiber.h)