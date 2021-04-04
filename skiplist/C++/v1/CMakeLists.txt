cmake_minimum_required(VERSION 3.16)
project(skiplist)

include_directories(include)

set(TEST_WITH_CATCH_SRC
)

set(MANUAL_TEST_SRC 
		tests/devtime_tests.cpp
	)

add_compile_options(-std=c++11 -g)
add_executable(manual_tests ${MANUAL_TEST_SRC})
#add_executable(test_with_catch ${TEST_WITH_CATCH_SRC})
