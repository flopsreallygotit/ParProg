#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

class Benchmark final {
  public:
    using benchmark_t = std::vector<int>;

    using sort_clock_t = std::chrono::high_resolution_clock;
    using sort_time_t = std::chrono::nanoseconds;

    Benchmark(std::size_t benchmark_size)
        : m_size(benchmark_size), m_benchmark(benchmark_size) {
        for (size_t i = 0; i < benchmark_size; ++i)
            m_benchmark[i] = std::rand();
    }

    void run_sort_impl(void (*sorter)(benchmark_t::iterator,
                                      benchmark_t::iterator),
                       const char *sorter_name) {
        sort_clock_t::time_point start = sort_clock_t::now();
        sorter(m_benchmark.begin(), m_benchmark.end());
        sort_clock_t::time_point end = sort_clock_t::now();

        long time =
            std::chrono::duration_cast<sort_time_t>(end - start).count();
        std::cout << "Time needed for `" << sorter_name << "` on size `"
                  << m_size << "`: `" << time << "` ns;\n";
    }

  private:
    std::size_t m_size;
    benchmark_t m_benchmark;
};

#define run_sort(sorter) run_sort_impl(sorter, #sorter)

int main() {
    Benchmark default_sorter(10000);
    default_sorter.run_sort(std::sort);
    default_sorter.run_sort(std::stable_sort);
}
