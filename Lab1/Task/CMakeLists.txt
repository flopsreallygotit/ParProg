cmake_minimum_required(VERSION 3.22)
project(TransportEquation)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(transport_equation transport_equation.cpp)
target_link_libraries(transport_equation ${MPI_CXX_LIBRARIES})

enable_testing()

add_test(NAME transport_equation_test
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2
           $<TARGET_FILE:transport_equation>
)
