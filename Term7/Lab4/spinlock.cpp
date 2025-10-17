#include "spinlock.hpp"
#include <iostream>
#include <thread>

SpinLock lock;

void increment_counter(int &counter, int operations) {
    for (int i = 0; i < operations; i++) {
        lock.lock();
        counter++;
        lock.unlock();
    }
}

int main() {
    constexpr const int thread_count = 8;
    constexpr const int operations_per_thread = 10000;

    int counter = 0;
    std::thread threads[thread_count];

    for (int i = 0; i < thread_count; i++)
        threads[i] = std::thread(increment_counter, std::ref(counter),
                                 operations_per_thread);

    for (int i = 0; i < thread_count; i++)
        threads[i].join();

    std::cout << "Counter value at the end: " << counter << '\n';

    if (counter != thread_count * operations_per_thread) {
        std::cerr << "Error: Counter value is incorrect" << '\n';
        return 1;
    }

    std::cout << "The counter value is correct.\n";

    return 0;
}
