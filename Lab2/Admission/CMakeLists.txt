cmake_minimum_required(VERSION 3.22)
project(ParallelSort)

add_executable(sort_compare sort_compare.cpp)
target_compile_options(sort_compare PRIVATE -O3 -pthread)

enable_testing()
add_test(
    NAME ParallelSortTest
    COMMAND sort_compare
)
