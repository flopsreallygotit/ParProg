#include <iostream>

#include "common_benchmark.hpp"

static void thread_communicator() {}

int main() {
    CommonBenchmark<thread_communicator> benchmark{};

    long avg_time = benchmark.get_avg_time();

    std::cout << "Average thread communication time is `" << avg_time
              << "` ns;\n";

    return 0;
}
