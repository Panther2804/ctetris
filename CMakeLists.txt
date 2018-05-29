cmake_minimum_required(VERSION 3.10)
project(tetrisrec)

set(CMAKE_CXX_STANDARD 11)

add_executable(tetrisrec main.cpp main.h treiber.cpp treiber.h)