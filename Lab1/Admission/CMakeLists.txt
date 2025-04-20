cmake_minimum_required(VERSION 3.22)
project(AdmissionMPI)

find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILER mpicxx)

add_executable(communication_time communication_time.cpp)
add_executable(pi_calculation pi_calculation.cpp)
