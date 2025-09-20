find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()

add_executable(strassen_parallel strassen.cpp)
target_link_libraries(strassen_parallel PUBLIC OpenMP::OpenMP_CXX)

add_executable(strassen strassen.cpp)

add_test(
    NAME strassen_test
    COMMAND strassen
)

add_test(
    NAME strassen_parallel_test
    COMMAND strassen_parallel
)
