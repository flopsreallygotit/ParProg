#include <cstddef>
#include <iostream>

#include "benchmark.hpp"
#include "matrix.hpp"

using TestMatrix = matrixes::StrassenMatrix<int>;

void standart_mul(TestMatrix lhs, TestMatrix rhs) {}

void strassen_mul(TestMatrix lhs, TestMatrix rhs) {}

template <auto Func> long test_mul(TestMatrix &lhs, TestMatrix &rhs) {
    static constexpr const auto trials_num = 10;

    auto benchmark =
        benchmarks::StaticBenchmark<Func, TestMatrix, TestMatrix>(trials_num);

    return benchmark.get_avg_time(lhs, rhs);
}

int main() {
    const std::size_t powers[] = {1, 5, 10, 15};

    for (const auto &power : powers) {
        auto lhs = TestMatrix(power);
        auto rhs = TestMatrix(power);

        lhs.randomize();
        rhs.randomize();

        long standart_time = test_mul<standart_mul>(lhs, rhs);
        long strassen_time = test_mul<strassen_mul>(lhs, rhs);
        double boost = static_cast<double>(strassen_time) / standart_time;

        std::cout << "Length `" << lhs.get_length() << "`; | Standart: `"
                  << standart_time << "` | Strassen: `" << strassen_time
                  << "` | Boost: `" << boost << "` |\n";
    }
}
