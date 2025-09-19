#include <iostream>

#include "benchmark.hpp"
#include "matrix.hpp"

using TestMatrix = matrixes::StrassenMatrix<int>;

int main() {
    static constexpr const auto trials_num = 5;

    const std::size_t powers[] = {5, 6, 7, 8};

    for (const auto &power : powers) {
        auto lhs = TestMatrix(power);
        auto rhs = TestMatrix(power);

        lhs.randomize();
        rhs.randomize();

        benchmarks::StaticBenchmark<TestMatrix::regular_multiply, TestMatrix &,
                                    TestMatrix &>
            regular_benchmark{trials_num};
        benchmarks::StaticBenchmark<TestMatrix::strassen_multiply, TestMatrix &,
                                    TestMatrix &>
            strassen_benchmark{trials_num};
        benchmarks::StaticBenchmark<TestMatrix::strassen_multiply_parallel,
                                    TestMatrix &, TestMatrix &>
            strassen_parallel_benchmark{trials_num};

        auto regular_time = regular_benchmark.get_avg_time(lhs, rhs);
        auto strassen_time = strassen_benchmark.get_avg_time(lhs, rhs);
        auto strassen_parallel_time =
            strassen_parallel_benchmark.get_avg_time(lhs, rhs);

        float boost = static_cast<float>(regular_time) / strassen_parallel_time;

        std::cout << "Power = `" << power << "` | Time(ns) Regular: `"
                  << regular_time << "`; Strassen: `" << strassen_time
                  << "`; Parallel: `" << strassen_parallel_time
                  << "`;\nBoost: `" << boost << "`;\n\n";
    }
}
