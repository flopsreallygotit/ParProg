#pragma once

#include <vector>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using benchmark_t = std::vector<int>;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace sorters {
benchmark_t sort(benchmark_t benchmark);
benchmark_t qsort(benchmark_t benchmark);
benchmark_t psort(benchmark_t benchmark);
} // namespace sorters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
