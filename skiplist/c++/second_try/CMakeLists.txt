cmake_minimum_required(VERSION 3.16)
project(skiplist)

include_directories(.)
include_directories(include)

set(TEST_WITH_CATCH_SRC
        tests/catch2_test.cpp
        tests/ctor.cpp
        tests/dtor.cpp
        tests/operator=.cpp
        tests/insert.cpp
        tests/erase.cpp
        tests/emplace.cpp
        tests/operator[].cpp
        tests/at.cpp
        tests/find.cpp
        tests/count.cpp
        tests/lower_bound.cpp
        tests/upper_bound.cpp
        tests/equal_range.cpp
        tests/swap.cpp
)

add_compile_options(-std=c++11 -g)
add_executable(test_with_catch ${TEST_WITH_CATCH_SRC})

add_executable(manual_test tests/manual_test.cpp)