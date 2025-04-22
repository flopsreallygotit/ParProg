cmake_minimum_required(VERSION 3.22)
project(ParallelSort)

add_executable(sort_compare sort_compare.cpp)
target_compile_options(sort_compare PRIVATE -O3 -pthread)

enable_testing()

add_test(
    NAME sort_compare_test
    COMMAND sort_compare
)
