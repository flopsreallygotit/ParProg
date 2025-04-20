#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "sorters.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T> class Benchmark final {
  public:
    using sort_clock_t = std::chrono::high_resolution_clock;
    using sort_time_t = std::chrono::nanoseconds;

    Benchmark(size_t benchmark_size, size_t trials_count = 10)
        : m_size(benchmark_size), m_trials_count(trials_count),
          m_benchmark(benchmark_size),
          m_stats("statistics.csv", std::ios::app) {
        std::generate(m_benchmark.begin(), m_benchmark.end(),
                      []() { return std::rand(); });

        m_sorted_benchmark = sorters::sort(m_benchmark);

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define test(sorter) test_impl(sorter, #sorter)

int main() {
    std::vector<size_t> benchmark_sizes = {1 << 5,  1 << 10, 1 << 15,
                                           1 << 20, 1 << 21, 1 << 22};

    for (size_t benchmark_size : benchmark_sizes) {
        Benchmark<int> current_benchmark{benchmark_size};

        current_benchmark.test(sorters::sort);
        current_benchmark.test(sorters::std_par_sort);
        current_benchmark.test(sorters::std_quick_sort);
        current_benchmark.test(sorters::custom_par_sort);
    }

    return 0;
}

#undef test

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
