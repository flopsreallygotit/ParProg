#include <algorithm>
#include <cstdlib>

#include "sorters.h"

namespace sorters {
benchmark_t sort(benchmark_t benchmark) {
    std::sort(benchmark.begin(), benchmark.end());

    return benchmark;
}

benchmark_t qsort(benchmark_t benchmark) {
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

benchmark_t psort(benchmark_t benchmark) { return benchmark; }
} // namespace sorters