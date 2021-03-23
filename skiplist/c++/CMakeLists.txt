cmake_minimum_required(VERSION 3.16)
project(skiplist)

include_directories(include)

set(TEST_WITH_CATCH_SRC
        tests/test_driver.cpp
)

add_compile_options(-std=c++11 -g)
add_executable(test_with_catch ${TEST_WITH_CATCH_SRC})

add_executable(manual_test tests/main.cpp)

add_executable(temp temp.cpp)