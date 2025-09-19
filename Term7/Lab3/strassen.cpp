#include <chrono>
#include <cstddef>
#include <iostream>

#include "benchmark.hpp"
#include "matrix.hpp"

using TestMatrix = matrixes::StrassenMatrix<int>;

// Wrappers for static benchmark
void regular_mul(TestMatrix lhs, TestMatrix rhs) {
    TestMatrix::regular_multiply(lhs, rhs);
}

void strassen_mul(TestMatrix lhs, TestMatrix rhs) {
    TestMatrix::strassen_multiply(lhs, rhs);
}

void strassen_parallel_mul(TestMatrix lhs, TestMatrix rhs) {
    TestMatrix::strassen_multiply_parallel(lhs, rhs);
}

int main() {
    static constexpr const auto trials_num = 5;

    const std::size_t powers[] = {5, 6, 7, 8};

    for (const auto &power : powers) {
        auto lhs = TestMatrix(power);
        auto rhs = TestMatrix(power);

        lhs.randomize();
        rhs.randomize();

        benchmarks::StaticBenchmark<regular_mul, TestMatrix &, TestMatrix &>
            regular_benchmark{trials_num};
        benchmarks::StaticBenchmark<strassen_mul, TestMatrix &, TestMatrix &>
            strassen_benchmark{trials_num};
        benchmarks::StaticBenchmark<strassen_parallel_mul, TestMatrix &,
                                    TestMatrix &>
            strassen_parallel_benchmark{trials_num};

        auto regular_time = regular_benchmark.get_avg_time(lhs, rhs);
        auto strassen_time = strassen_benchmark.get_avg_time(lhs, rhs);
        auto strassen_parallel_time =
            strassen_parallel_benchmark.get_avg_time(lhs, rhs);

        float boost = static_cast<float>(regular_time) / strassen_parallel_time;

        std::cout << "Power = `" << power << "` | Time(ns) Regular: `" << regular_time
                  << "`; Strassen: `" << strassen_time << "`; Parallel: `"
                  << strassen_parallel_time << "`;\nBoost: `" << boost
                  << "`;\n\n";
    }
}
