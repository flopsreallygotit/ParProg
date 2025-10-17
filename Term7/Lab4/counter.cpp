#include <iostream>
#include <latch>
#include <syncstream>
#include <thread>
#include <vector>

#include "counter.hpp"

int main() {
    constexpr const int max_value = 50;
    constexpr const int thread_count = 8;
    constexpr const int iterations_per_thread = 15;

    BoundedCounter counter(max_value);
    std::latch latch(thread_count);
    std::vector<std::thread> threads;

    std::atomic<int> successful_operations = 0;
    std::atomic<int> failed_operations = 0;

    std::cout << "Starting concurrent test...\n";
    std::cout << "Max counter value: " << max_value << "\n";
    std::cout << "Threads: " << thread_count << "\n";
    std::cout << "Iterations per thread: " << iterations_per_thread << "\n\n";

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&, thread_id = i] {
            latch.arrive_and_wait();

            for (int j = 0; j < iterations_per_thread; ++j) {
                if (counter.try_increment()) {
                    successful_operations.fetch_add(1,
                                                    std::memory_order_relaxed);
                    std::osyncstream(std::cout)
                        << "Thread " << thread_id << ": incremented to "
                        << counter.value() << "\n";
                } else {
                    failed_operations.fetch_add(1, std::memory_order_relaxed);
                    std::osyncstream(std::cout)
                        << "Thread " << thread_id << ": failed to increment ("
                        << counter.value() << "/" << counter.max_value()
                        << ")\n";
                }
            }
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << "\n=== Test Results ===\n";
    std::cout << "Final counter value: " << counter.value() << "/"
              << counter.max_value() << "\n";
    std::cout << "Successful operations: " << successful_operations << "\n";
    std::cout << "Failed operations: " << failed_operations << "\n";
    std::cout << "Total attempts: "
              << (successful_operations + failed_operations) << "\n";

    std::cout << "\n=== Integrity Check ===\n";
    std::cout << "Counter never exceeded max: "
              << (counter.value() <= counter.max_value()) << "\n";
    std::cout << "Counter is non-negative: " << (counter.value() >= 0) << "\n";

    return 0;
}
