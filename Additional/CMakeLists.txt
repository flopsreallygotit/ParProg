cmake_minimum_required(VERSION 3.22)
project(Additional)

add_executable(sequential_access sequential_access.cpp)
target_compile_options(sequential_access PRIVATE -O3 -pthread)
target_compile_definitions(sequential_access PRIVATE $<$<CONFIG:Debug>:DEBUG>)

enable_testing()
add_test(NAME sequential_access_test
    COMMAND sequential_access
)
