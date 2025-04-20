#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <execution>
#include <thread>

#include "sorters.h"

namespace sorters::parallel_sorter { // TODO[flops]: Rename benchmark

class ParrallelSorter final {
  public:
    ParrallelSorter(unsigned int threads_num)
        : m_threads_num(threads_num), threads(threads_num) {}

    void sort(benchmark_t &m_target) {
        // size_t chunk_size = m_target.size() / m_threads_num;
        // size_t remaining  = m_target.size() % m_threads_num;

        // size_t start = 0;
        // for (size_t i = 0; i < m_threads_num; ++i) {
        //     size_t end = start + chunk_size + (i < remaining ? 1 : 0);

        //     threads.emplace_back();

        //     start = end;
        // }

        // for (std::thread &thread : threads)
        //     thread.join();
    }

  private:
    void worker_sort(benchmark_t &target, size_t start, size_t end) {
        std::sort(target.begin() + start, target.begin() + end);
    }

    unsigned int m_threads_num = 0;
    std::vector<std::thread> threads;
};

} // namespace sorters::parallel_sorter

namespace sorters {

benchmark_t sort(benchmark_t benchmark) {
    std::sort(benchmark.begin(), benchmark.end());

    return benchmark;
}

benchmark_t par_exec_sort(benchmark_t benchmark) {
    std::sort(std::execution::par, benchmark.begin(), benchmark.end());

    return benchmark;
}

benchmark_t quick_sort(benchmark_t benchmark) {
    std::qsort(benchmark.data(), benchmark.size(),
               sizeof(decltype(benchmark)::value_type),
               [](const void *x, const void *y) {
                   const int arg1 = *static_cast<const int *>(x);
                   const int arg2 = *static_cast<const int *>(y);

                   if (arg1 < arg2)
                       return -1;
                   if (arg1 > arg2)
                       return 1;

                   return 0;
               });

    return benchmark;
}

benchmark_t parallel_sort(benchmark_t benchmark) {
    parallel_sorter::ParrallelSorter sorter{
        std::thread::hardware_concurrency()};

    sorter.sort(benchmark);

    return benchmark;
}

} // namespace sorters