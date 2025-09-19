#include <cstddef>
#include <iostream>

#include "matrix.hpp"

using TestMatrix = matrixes::StrassenMatrix<int>;

// Wrappers for static benchmark
void regular_mul(TestMatrix lhs, TestMatrix rhs) {
    TestMatrix::regular_multiply(lhs, rhs);
}

void strassen_mul(TestMatrix lhs, TestMatrix rhs) {
    TestMatrix::strassen_multiply(lhs, rhs);
}

int main() {
    const std::size_t powers[] = {5};

    for (const auto &power : powers) {
        auto lhs = TestMatrix(power);
        auto rhs = TestMatrix(power);

        lhs.randomize();
        rhs.randomize();

        auto regular = TestMatrix::regular_multiply(lhs, rhs);
        auto strassen = TestMatrix::strassen_multiply(lhs, rhs);
        auto strassen_parallel =
            TestMatrix::strassen_multiply_parallel(lhs, rhs);

        bool equal = (regular == strassen) && (strassen == strassen_parallel);

        std::cout << equal;
    }
}
