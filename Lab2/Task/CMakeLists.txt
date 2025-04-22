cmake_minimum_required(VERSION 3.22)
project(InterprocessCommunication)

add_executable(pipe_communication pipe_communication.cpp)
target_compile_options(pipe_communication PRIVATE -O3)
target_compile_definitions(pipe_communication PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_executable(thread_communication thread_communication.cpp)
