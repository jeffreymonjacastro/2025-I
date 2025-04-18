cmake_minimum_required(VERSION 3.30)
project(Rtree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Rtree
    main.cpp
    Rtree.h)
