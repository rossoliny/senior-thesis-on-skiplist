cmake_minimum_required(VERSION 3.16)
project(benchmark_with_google)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS}             \
        -g                              \
        -o2                             \
        -mno-omit-leaf-frame-pointer    \
        -fno-omit-frame-pointer         \ ")

set(INCLUDE_DIRS
        ../../skiplist/C++/v2/include
        utils
    )

set(BENCH_WITH_GOOGLE_SRC
        utils/bench_utils.cpp
        src/ctor/empty_ctor.cpp
        src/ctor/range_ctor.cpp
        src/ctor/copy_ctor.cpp
        src/ctor/move_ctor.cpp
        src/erase/erase.cpp
        src/erase/erase_range.cpp
        src/erase/clear.cpp
        src/swap/swap.cpp
        src/find/find.cpp
        src/assignment/copy_assignment.cpp
        src/assignment/move_assignment.cpp
    )

set(STD_MAP_BENCH std_map_bench)
set(SKIPLIST_BENCH skiplist_bench)

find_package(benchmark REQUIRED)

add_executable(${STD_MAP_BENCH} ${BENCH_WITH_GOOGLE_SRC})
target_compile_definitions(${STD_MAP_BENCH} PUBLIC STD_MAP_BENCH)
target_include_directories(${STD_MAP_BENCH} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${STD_MAP_BENCH} benchmark::benchmark_main)


add_executable(${SKIPLIST_BENCH} ${BENCH_WITH_GOOGLE_SRC})
target_compile_definitions(${SKIPLIST_BENCH} PUBLIC SKIPLIST_BENCH)
target_include_directories(${SKIPLIST_BENCH} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${SKIPLIST_BENCH} benchmark::benchmark_main)