add_executable(counter counter.cpp)

add_test(
    NAME counter_test
    COMMAND counter
)
