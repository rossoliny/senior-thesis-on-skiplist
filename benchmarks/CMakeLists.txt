cmake_minimum_required(VERSION 3.16)
project(benchmarks)

set(CMAKE_CXX_STANDARD 11)
include_directories(../skiplist/C++/v2/include)

add_compile_options(-fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -O0 -g)

add_executable(benchmarks manual.cpp)
