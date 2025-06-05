cmake_minimum_required(VERSION 3.30)
project(SRtree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SRtree
    main2.cpp
    MBB.h
    Point.h
    Sphere.h
    SRtree.h)
