cmake_minimum_required(VERSION 3.10)

project(EDA)

set(CMAKE_CXX_STANDARD 20)

include_directories(W1)

add_executable(EDA W1/main.cpp)
