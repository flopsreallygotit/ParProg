#include <atomic>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "benchmark.hpp"

double f(double x) { return std::sin(1 / x); }

double derivative_f(double x) { return std::cos(1 / x) * (-1 / (x * x)); }

double adaptive_step(double x, double base_step) {
    return base_step / (1 + 10 * std::abs(derivative_f(x)));
}

void worker(double a, double b, int total_intervals,
            std::atomic<int> &next_interval, double &result,
            std::mutex &result_mutex, double base_step) {
    double local_sum = 0;

    while (true) {
        int i = next_interval.fetch_add(1);
        if (i >= total_intervals)
            break;

        double x = a + (b - a) * i / total_intervals;
        double next_x = a + (b - a) * (i + 1) / total_intervals;
        double dx = next_x - x;

        double step = adaptive_step(x, dx);
        int sub_steps = std::max(1, static_cast<int>(std::ceil(dx / step)));
        double sub_dx = dx / sub_steps;

        for (int j = 0; j < sub_steps; ++j) {
            double sub_x = x + j * sub_dx;
            double mid = sub_x + sub_dx / 2;
            local_sum += f(mid) * sub_dx;
        }
    }

    std::lock_guard<std::mutex> lock(result_mutex);
    result += local_sum;
}

void parallel_integrate(double a, double b, int num_threads,
                        int total_intervals) {
    std::atomic<int> next_interval(0);
    std::mutex result_mutex;

    double result = 0;
    double base_step = (b - a) / total_intervals;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
        threads.emplace_back(worker, a, b, total_intervals,
                             std::ref(next_interval), std::ref(result),
                             std::ref(result_mutex), base_step);

    for (auto &t : threads)
        t.join();

    std::cout << "The result is: `" << result << "`\n";
}

int main() {
    double a = 0.1;
    double b = 1;
    int num_threads = std::thread::hardware_concurrency();
    int total_intervals = 1000000;

    benchmarks::StaticBenchmark<parallel_integrate, double, double, int, int>
        benchmark{5};
    long time = benchmark.get_avg_time(a, b, num_threads, total_intervals);

    std::cout << "Average time is `" << time << "` ns\n";

    return 0;
}