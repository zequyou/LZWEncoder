cmake_minimum_required(VERSION 3.15)
project(LZWEncoder)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(LZWEncoder main.cpp)