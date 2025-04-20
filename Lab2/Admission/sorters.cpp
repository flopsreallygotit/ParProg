#include <algorithm>
#include <cstdlib>
#include <execution>
#include <thread>

#include "sorters.h"

namespace sorters {

benchmark_t sort(benchmark_t benchmark) {
    std::sort(benchmark.begin(), benchmark.end());

    return benchmark;
}

benchmark_t std_par_sort(benchmark_t benchmark) {
    std::sort(std::execution::par, benchmark.begin(), benchmark.end());

    return benchmark;
}

benchmark_t std_quick_sort(benchmark_t benchmark) {
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

static int partition(benchmark_t &target, int left, int right) {
    int separator = target[right], i = left - 1;

    for (int j = left; j < right; ++j)
        if (target[j] < separator)
            std::swap(target[++i], target[j]);

    std::swap(target[i + 1], target[right]);

    return i + 1;
}

static void par_qsort(benchmark_t &target, int left, int right,
                      unsigned depth = 0) {
    if (left >= right)
        return;

    ++depth;

    int separator = partition(target, left, right);
    if (depth <= 8) {
        std::thread left_sorter{par_qsort, std::ref(target), left,
                                separator - 1, depth};

        par_qsort(target, separator + 1, right, depth);

        left_sorter.join();
    }

    else {
        par_qsort(target, left, separator - 1, depth);
        par_qsort(target, separator + 1, right, depth);
    }
}

benchmark_t custom_par_sort(benchmark_t benchmark) {
    par_qsort(benchmark, 0, benchmark.size() - 1, 0);

    return benchmark;
}

} // namespace sorters