cmake_minimum_required(VERSION 3.26)
project(lab_bfs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab_bfs
        bfs.cpp
        bfs.h
        main.cpp
        Profiler.h)
