#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "sorters.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Benchmark final {
  public:
    using sort_clock_t = std::chrono::high_resolution_clock;
    using sort_time_t = std::chrono::nanoseconds;

    Benchmark(size_t benchmark_size)
        : m_size(benchmark_size), m_benchmark(benchmark_size) {
        std::generate(m_benchmark.begin(), m_benchmark.end(),
                      []() { return std::rand(); });

        m_sorted_benchmark = sorters::sort(m_benchmark);
    }

    bool sort_succeed(benchmark_t &sort_result) {
        return sort_result == m_sorted_benchmark;
    }

    void run_sort_impl(benchmark_t (*sorter)(benchmark_t benchmark_copy),
                       const char *sorter_name) {
        sort_clock_t::time_point start = sort_clock_t::now();
        benchmark_t sort_result =
            sorter(m_benchmark); // Copies benchmark because we need to rerun
                                 // new funcs on same benchmark
        sort_clock_t::time_point end = sort_clock_t::now();

        long time =
            std::chrono::duration_cast<sort_time_t>(end - start).count();

        std::cout << (sort_succeed(sort_result) ? "SUCCESS" : "FAIL") << ": `"
                  << sorter_name << "` on size `" << m_size << "`: `" << time
                  << "` ns;\n";
    }

  private:
    size_t m_size;

    benchmark_t m_benchmark;
    benchmark_t m_sorted_benchmark;
};

#define run_sort(sorter) run_sort_impl(sorter, #sorter)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main() {
    std::vector<size_t> benchmark_sizes = {100000, 10000000};

    for (size_t benchmark_size : benchmark_sizes) {
        Benchmark current_benchmark{benchmark_size};

        current_benchmark.run_sort(sorters::par_exec_sort);
        current_benchmark.run_sort(sorters::quick_sort);
        current_benchmark.run_sort(sorters::parallel_sort);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
