cmake_minimum_required(VERSION 3.21)
project(FM)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(FM
        src/FMptr.cpp
        src/FMptr.h
        src/common.cpp
        src/common.h
        )
