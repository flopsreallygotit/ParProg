add_executable(counter counter.cpp)

add_test(
    NAME counter_test
    COMMAND counter
)

add_executable(spinlock spinlock.cpp)

add_test(
    NAME spinlock_test
    COMMAND spinlock
)
