cmake_minimum_required(VERSION 3.24)
project(lineAlgorithmBresenham)

set(CMAKE_CXX_STANDARD 17)

add_executable(lineAlgorithmBresenham main.cpp includes/filling/filling.cpp includes/hermit/hermit.cpp ../../computer_graphics/Line.h ../../computer_graphics/Line.h)
