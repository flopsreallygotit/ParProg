#pragma once

#include <vector>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using benchmark_t = std::vector<int>;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace sorters {
benchmark_t sort(benchmark_t benchmark);
benchmark_t par_exec_sort(benchmark_t benchmark);
benchmark_t quick_sort(benchmark_t benchmark);
benchmark_t parallel_sort(benchmark_t benchmark);
} // namespace sorters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
