#pragma once

#include <algorithm>
#include <cstdlib>
#include <execution>
#include <thread>

namespace sorters {

template <typename T>
std::vector<T, std::allocator<T>>
sort(std::vector<T, std::allocator<T>> benchmark) {
    std::sort(benchmark.begin(), benchmark.end());

    return benchmark;
}

template <typename T>
std::vector<T, std::allocator<T>>
std_par_sort(std::vector<T, std::allocator<T>> benchmark) {
    std::sort(std::execution::par, benchmark.begin(), benchmark.end());

    return benchmark;
}

template <typename T>
static int qsort_compare(const void *lhs, const void *rhs) {
    const T casted_lhs = *static_cast<const T *>(lhs);
    const T casted_rhs = *static_cast<const T *>(rhs);

    if (lhs < rhs)
        return -1;
    else if (lhs > rhs)
        return 1;

    return 0;
}

template <typename T>
std::vector<T, std::allocator<T>>
std_quick_sort(std::vector<T, std::allocator<T>> benchmark) {
    std::qsort(benchmark.data(), benchmark.size(), sizeof(T), qsort_compare<T>);

    return benchmark;
}

template <typename T>
static int sort_part(std::vector<T, std::allocator<T>> &target, ssize_t left,
                     ssize_t right) {
    int separator = target[right];
    ssize_t i = left - 1;

    for (ssize_t j = left; j < right; ++j)
        if (target[j] < separator)
            std::swap(target[++i], target[j]);

    std::swap(target[i + 1], target[right]);

    return i + 1;
}

template <typename T>
static void par_qsort(std::vector<T, std::allocator<T>> &target, ssize_t left,
                      ssize_t right, unsigned depth = 0) {
    if (left >= right)
        return;

    ++depth;

    ssize_t separator = sort_part(target, left, right);
    if (depth <= 8) {
        std::thread left_sorter{par_qsort<T>, std::ref(target), left,
                                separator - 1, depth};

        par_qsort(target, separator + 1, right, depth);

        left_sorter.join();
    }

    else {
        par_qsort(target, left, separator - 1, depth);
        par_qsort(target, separator + 1, right, depth);
    }
}

template <typename T>
std::vector<T, std::allocator<T>>
custom_par_sort(std::vector<T, std::allocator<T>> benchmark) {
    par_qsort(benchmark, 0, benchmark.size() - 1);

    return benchmark;
}

} // namespace sorters