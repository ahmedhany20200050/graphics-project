cmake_minimum_required(VERSION 3.24)
project(lineAlgorithmBresenham)

set(CMAKE_CXX_STANDARD 17)

add_executable(lineAlgorithmBresenham main.cpp includes/filling/filling.h Line.cpp includes/filling/filling.cpp)
