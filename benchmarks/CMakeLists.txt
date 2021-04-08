cmake_minimum_required(VERSION 3.16)
project(Benchmarks)

set(CMAKE_CXX_STANDARD 17)
include_directories(../skiplist/C++/v2/include)

add_executable(Benchmarks main.cpp)