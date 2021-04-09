cmake_minimum_required(VERSION 3.16)
project(benchmark_with_google)

set(CMAKE_CXX_STANDARD 11)


set(BENCH_WITH_GOOGLE_SRC
        main.cpp

    )

find_package(benchmark REQUIRED)

add_executable(${PROJECT_NAME} ${BENCH_WITH_GOOGLE_SRC})
target_link_libraries(${PROJECT_NAME} benchmark::benchmark_main)