cmake_minimum_required(VERSION 3.22)
project(ThreadsAndProcesses)

add_executable(sort_compare sort_compare.cpp)
target_compile_options(sort_compare PRIVATE -O3 -pthread)

add_executable(pipe_communication pipe_communication.cpp)
target_compile_options(pipe_communication PRIVATE -O3)
target_compile_definitions(pipe_communication PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_executable(thread_communication thread_communication.cpp)
target_compile_options(thread_communication PRIVATE -O3)
target_compile_definitions(thread_communication PRIVATE $<$<CONFIG:Debug>:DEBUG>)

enable_testing()

add_test(
    NAME sort_compare_test
    COMMAND sort_compare
)

add_test(
    NAME pipe_communication_test
    COMMAND pipe_communication
)

add_test(
    NAME thread_communication_test
    COMMAND thread_communication
)
