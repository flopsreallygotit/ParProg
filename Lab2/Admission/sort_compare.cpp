#include "benchmark.hpp"
#include "sorters.hpp"

#define test(sorter) test_impl(sorter, #sorter)

int main() {
    std::vector<size_t> benchmark_sizes = {1 << 5,  1 << 10, 1 << 15,
                                           1 << 20, 1 << 21, 1 << 22};

    for (size_t benchmark_size : benchmark_sizes) {
        benchmarks::SortBenchmark<int> current_benchmark{benchmark_size};

        current_benchmark.test(sorters::sort);
        current_benchmark.test(sorters::std_par_sort);
        current_benchmark.test(sorters::std_quick_sort);
        current_benchmark.test(sorters::custom_par_sort);
    }

    return 0;
}

#undef test
