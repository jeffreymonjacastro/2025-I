cmake_minimum_required(VERSION 3.30)
project(Mtree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Mtree
    main.cpp
    Mtree.h
    Object.h)
