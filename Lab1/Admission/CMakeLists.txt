cmake_minimum_required(VERSION 3.22)
project(AdmissionMPI)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(mpi_communication mpi_communication.cpp)
target_link_libraries(mpi_communication ${MPI_CXX_LIBRARIES})

add_executable(pi_calculation pi_calculation.cpp)
target_link_libraries(pi_calculation ${MPI_CXX_LIBRARIES})

enable_testing()
add_test(NAME pi_test
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2
           $<TARGET_FILE:pi_calculation>
)

add_test(NAME communication_test
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2
           $<TARGET_FILE:mpi_communication>
)