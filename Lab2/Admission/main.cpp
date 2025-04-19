#include <chrono>
#include <cstdlib>
#include <iostream>

#include "sorters.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Benchmark final {
  public:
    using sort_clock_t = std::chrono::high_resolution_clock;
    using sort_time_t = std::chrono::nanoseconds;

    Benchmark(std::size_t benchmark_size)
        : m_size(benchmark_size), m_benchmark(benchmark_size) {
        for (size_t i = 0; i < benchmark_size; ++i)
            m_benchmark[i] = std::rand();

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
    std::size_t m_size;

    benchmark_t m_benchmark;
    benchmark_t m_sorted_benchmark;
};

#define run_sort(sorter) run_sort_impl(sorter, #sorter)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main() {
    std::vector<std::size_t> benchmark_sizes = {10, 100, 1000, 10000, 100000};

    for (std::size_t benchmark_size : benchmark_sizes) {
        Benchmark current_benchmark(benchmark_size);

        current_benchmark.run_sort(sorters::qsort);
        current_benchmark.run_sort(sorters::psort);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
