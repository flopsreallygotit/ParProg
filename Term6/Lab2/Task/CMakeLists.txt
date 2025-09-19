cmake_minimum_required(VERSION 3.22)
project(ParallelIntegralComputation)

add_executable(parallel_integral parallel_integral.cpp)
target_compile_options(parallel_integral PRIVATE -O3 -pthread)

enable_testing()

add_test(
    NAME parallel_integral_test
    COMMAND parallel_integral
)
