#pragma once

#include <vector>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using benchmark_t = std::vector<int>; // TODO[flops]: templates

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace sorters {
benchmark_t sort(benchmark_t benchmark);
benchmark_t std_par_sort(benchmark_t benchmark);
benchmark_t std_quick_sort(benchmark_t benchmark);
benchmark_t custom_par_sort(benchmark_t benchmark);
} // namespace sorters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
