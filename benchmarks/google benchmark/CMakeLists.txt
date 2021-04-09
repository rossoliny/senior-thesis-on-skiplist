cmake_minimum_required(VERSION 3.16)
project(benchmark_with_google)

set(CMAKE_CXX_STANDARD 11)

set(STD_MAP_BENCH rbtree_bench)
set(SKIPLIST_BENCH skiplist_bench)

include_directories(utils)

set(INCLUDE_DIRS
        ../../skiplist/C++/v2/include
    )

set(BENCH_WITH_GOOGLE_SRC
        ctor/empty_ctor.cpp
        ctor/range_ctor.cpp
    )

find_package(benchmark REQUIRED)

add_executable(${STD_MAP_BENCH} ${BENCH_WITH_GOOGLE_SRC})
add_executable(${SKIPLIST_BENCH} ${BENCH_WITH_GOOGLE_SRC})

target_compile_definitions(${STD_MAP_BENCH} PUBLIC STD_MAP_BENCH)
target_compile_definitions(${SKIPLIST_BENCH} PUBLIC SKIPLIST_BENCH)

target_include_directories(${STD_MAP_BENCH} PUBLIC ${INCLUDE_DIRS})
target_include_directories(${SKIPLIST_BENCH} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(${STD_MAP_BENCH} benchmark::benchmark_main)
target_link_libraries(${SKIPLIST_BENCH} benchmark::benchmark_main)