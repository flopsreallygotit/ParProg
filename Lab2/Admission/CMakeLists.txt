cmake_minimum_required(VERSION 3.22)
project(ParallelSort)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -O3 -pthread)

enable_testing()
add_test(
    NAME ParallelSortTest
    COMMAND main
)
