#include <atomic>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "benchmark.hpp"

int main() {
    double a = 0.1;
    double b = 1.0;
    int num_threads = std::thread::hardware_concurrency();

    // benchmarks::StaticBenchmark<thread_communicator, StringBuffer *>
    // benchmark{};

    // long avg_time = benchmark.get_avg_time(common_buffer);
    // std::cout << "Average thread communication time is `" << avg_time
    //           << "` ns;\n";

    return 0;
}