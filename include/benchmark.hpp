#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace benchmarks {

template <auto Func, typename... Args> class StaticBenchmark final {
  public:
    StaticBenchmark(unsigned trials_count = 100)
        : m_trials_count(trials_count) {}

    long get_avg_time(Args &...args) {
        using clock_t = std::chrono::high_resolution_clock;
        using time_t = std::chrono::nanoseconds;

        long sum_time = 0;
        for (unsigned trial = 0; trial < m_trials_count; ++trial) {
            clock_t::time_point start = clock_t::now();
            Func(std::forward<Args>(args)...);
            clock_t::time_point end = clock_t::now();

            sum_time += std::chrono::duration_cast<time_t>(end - start).count();
        }

        return (sum_time / m_trials_count);
    }

  private:
    unsigned m_trials_count = 0;
};

template <typename T> class SortBenchmark final {
  public:
    SortBenchmark(size_t benchmark_size, size_t trials_count = 10)
        : m_size(benchmark_size), m_trials_count(trials_count),
          m_benchmark(benchmark_size),
          m_stats("statistics.csv", std::ios::app) {
        std::generate(m_benchmark.begin(), m_benchmark.end(),
                      []() { return std::rand(); });

        m_sorted_benchmark = m_benchmark;
        std::sort(m_sorted_benchmark.begin(), m_sorted_benchmark.end());

        if (m_stats.tellp() == 0)
            m_stats << "Size,Function,Time\n";

        std::cout << "Initialized <" << typeid(T).name()
                  << "> benchmark with size `" << m_size << "`\n";
    }

    void test_impl(std::vector<T> (*sorter)(std::vector<T> benchmark_copy),
                   const char *sorter_name) {
        for (size_t trial = 0; trial < m_trials_count; ++trial)
            run_sort(sorter, sorter_name);
    }

  private:
    bool sort_succeed(std::vector<T> &sort_result) {
        return sort_result == m_sorted_benchmark;
    }

    void run_sort(std::vector<T> (*sorter)(std::vector<T> benchmark_copy),
                  const char *sorter_name) {
        using sort_clock_t = std::chrono::high_resolution_clock;
        using sort_time_t = std::chrono::nanoseconds;

        sort_clock_t::time_point start = sort_clock_t::now();
        std::vector<T> sort_result =
            sorter(m_benchmark); // Copies benchmark because we need to rerun
                                 // new funcs on same benchmark
        sort_clock_t::time_point end = sort_clock_t::now();

        long time =
            std::chrono::duration_cast<sort_time_t>(end - start).count();

        if (sort_succeed(sort_result))
            m_stats << m_size << ',' << sorter_name << ',' << time << '\n';
        else
            std::cerr << "Fail on `" << sorter_name << "`\n";
    }

    size_t m_size;
    size_t m_trials_count;
    std::ofstream m_stats;
    std::vector<T> m_benchmark;
    std::vector<T> m_sorted_benchmark;
};

} // namespace benchmarks
