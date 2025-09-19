#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include <omp.h>

namespace matrixes {

template <typename T> class StrassenMatrix {
    static constexpr const auto regular_transfer_power = 3;

    std::vector<T> m_intra;
    std::size_t m_length;
    std::size_t m_power;

  public:
    StrassenMatrix(std::size_t power) : m_power(power) {
        m_length = std::pow(2, power);
        m_intra.resize(m_length * m_length);
    }

    std::size_t get_length() const { return m_length; }
    std::size_t get_power() const { return m_power; }

    void randomize() {
        std::generate(m_intra.begin(), m_intra.end(),
                      []() { return std::rand(); });
    }

    T &operator()(std::size_t x, std::size_t y) {
        if (x >= m_length || y >= m_length)
            throw std::out_of_range("Matrix indices out of range");

        return m_intra[m_length * y + x];
    }

    const T &operator()(std::size_t x, std::size_t y) const {
        if (x >= m_length || y >= m_length)
            throw std::out_of_range("Matrix indices out of range");

        return m_intra[m_length * y + x];
    }

    StrassenMatrix operator+(const StrassenMatrix &other) const {
        if (m_power != other.m_power)
            throw std::invalid_argument(
                "Matrix powers must be equal for addition");

        StrassenMatrix result(m_power);
        for (std::size_t i = 0; i < m_intra.size(); ++i)
            result.m_intra[i] = m_intra[i] + other.m_intra[i];

        return result;
    }

    StrassenMatrix operator-(const StrassenMatrix &other) const {
        if (m_power != other.m_power)
            throw std::invalid_argument(
                "Matrix powers must be equal for subtraction");

        StrassenMatrix result(m_power);
        for (std::size_t i = 0; i < m_intra.size(); ++i)
            result.m_intra[i] = m_intra[i] - other.m_intra[i];

        return result;
    }

    bool operator==(const StrassenMatrix &other) const {
        if (m_power != other.m_power)
            return false;
        for (std::size_t i = 0; i < m_intra.size(); ++i)
            if (m_intra[i] != other.m_intra[i])
                return false;
        return true;
    }

    static StrassenMatrix regular_multiply(const StrassenMatrix &A,
                                           const StrassenMatrix &B) {
        if (A.m_power != B.m_power)
            throw std::invalid_argument(
                "Matrix powers must be equal for multiplication");

        StrassenMatrix result(A.m_power);
        std::size_t n = A.m_length;

        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                for (std::size_t k = 0; k < n; ++k)
                    result(j, i) += A(k, i) * B(j, k);

        return result;
    }

    static StrassenMatrix strassen_multiply(const StrassenMatrix &A,
                                            const StrassenMatrix &B) {
        if (A.m_power != B.m_power)
            throw std::invalid_argument(
                "Matrix powers must be equal for multiplication");

        if (A.m_power <= regular_transfer_power)
            return regular_multiply(A, B);

        std::size_t new_power = A.m_power - 1;
        std::size_t half_size = A.m_length / 2;

        auto extract_quadrant = [](const StrassenMatrix &mat,
                                   std::size_t start_x, std::size_t start_y,
                                   std::size_t size) {
            StrassenMatrix<T> result(mat.m_power - 1);

            for (std::size_t y = 0; y < size; ++y)
                for (std::size_t x = 0; x < size; ++x)
                    result(x, y) = mat(start_x + x, start_y + y);

            return result;
        };

        StrassenMatrix A11 = extract_quadrant(A, 0, 0, half_size);
        StrassenMatrix A12 = extract_quadrant(A, half_size, 0, half_size);
        StrassenMatrix A21 = extract_quadrant(A, 0, half_size, half_size);
        StrassenMatrix A22 =
            extract_quadrant(A, half_size, half_size, half_size);

        StrassenMatrix B11 = extract_quadrant(B, 0, 0, half_size);
        StrassenMatrix B12 = extract_quadrant(B, half_size, 0, half_size);
        StrassenMatrix B21 = extract_quadrant(B, 0, half_size, half_size);
        StrassenMatrix B22 =
            extract_quadrant(B, half_size, half_size, half_size);

        StrassenMatrix M1 = strassen_multiply(A11 + A22, B11 + B22);
        StrassenMatrix M2 = strassen_multiply(A21 + A22, B11);
        StrassenMatrix M3 = strassen_multiply(A11, B12 - B22);
        StrassenMatrix M4 = strassen_multiply(A22, B21 - B11);
        StrassenMatrix M5 = strassen_multiply(A11 + A12, B22);
        StrassenMatrix M6 = strassen_multiply(A21 - A11, B11 + B12);
        StrassenMatrix M7 = strassen_multiply(A12 - A22, B21 + B22);

        StrassenMatrix C11 = M1 + M4 - M5 + M7;
        StrassenMatrix C12 = M3 + M5;
        StrassenMatrix C21 = M2 + M4;
        StrassenMatrix C22 = M1 - M2 + M3 + M6;

        StrassenMatrix result(A.m_power);

        auto insert_quadrant =
            [](StrassenMatrix &result, const StrassenMatrix &quadrant,
               std::size_t start_x, std::size_t start_y, std::size_t size) {
                for (std::size_t y = 0; y < size; ++y)
                    for (std::size_t x = 0; x < size; ++x)
                        result(start_x + x, start_y + y) = quadrant(x, y);
            };

        insert_quadrant(result, C11, 0, 0, half_size);
        insert_quadrant(result, C12, half_size, 0, half_size);
        insert_quadrant(result, C21, 0, half_size, half_size);
        insert_quadrant(result, C22, half_size, half_size, half_size);

        return result;
    }

    static StrassenMatrix strassen_multiply_parallel(const StrassenMatrix &A,
                                                     const StrassenMatrix &B) {
        if (A.m_power != B.m_power)
            throw std::invalid_argument(
                "Matrix powers must be equal for multiplication");

        if (A.m_power <= regular_transfer_power)
            return regular_multiply(A, B);

        std::size_t new_power = A.m_power - 1;
        std::size_t half_size = A.m_length / 2;

        auto extract_quadrant = [](const StrassenMatrix &mat,
                                   std::size_t start_x, std::size_t start_y,
                                   std::size_t size) {
            StrassenMatrix<T> result(mat.m_power - 1);
#pragma omp parallel for collapse(2)
            for (std::size_t y = 0; y < size; ++y) {
                for (std::size_t x = 0; x < size; ++x) {
                    result(x, y) = mat(start_x + x, start_y + y);
                }
            }
            return result;
        };

        StrassenMatrix A11(new_power), A12(new_power), A21(new_power),
            A22(new_power);
        StrassenMatrix B11(new_power), B12(new_power), B21(new_power),
            B22(new_power);

#pragma omp parallel sections
        {
#pragma omp section
            {
                A11 = extract_quadrant(A, 0, 0, half_size);
            }
#pragma omp section
            {
                A12 = extract_quadrant(A, half_size, 0, half_size);
            }
#pragma omp section
            {
                A21 = extract_quadrant(A, 0, half_size, half_size);
            }
#pragma omp section
            {
                A22 = extract_quadrant(A, half_size, half_size, half_size);
            }

#pragma omp section
            {
                B11 = extract_quadrant(B, 0, 0, half_size);
            }
#pragma omp section
            {
                B12 = extract_quadrant(B, half_size, 0, half_size);
            }
#pragma omp section
            {
                B21 = extract_quadrant(B, 0, half_size, half_size);
            }
#pragma omp section
            {
                B22 = extract_quadrant(B, half_size, half_size, half_size);
            }
        }

        StrassenMatrix M1(new_power), M2(new_power), M3(new_power),
            M4(new_power);
        StrassenMatrix M5(new_power), M6(new_power), M7(new_power);

#pragma omp parallel
#pragma omp single nowait
        {
#pragma omp task shared(M1)
            {
                M1 = strassen_multiply_parallel(A11 + A22, B11 + B22);
            }

#pragma omp task shared(M2)
            {
                M2 = strassen_multiply_parallel(A21 + A22, B11);
            }

#pragma omp task shared(M3)
            {
                M3 = strassen_multiply_parallel(A11, B12 - B22);
            }

#pragma omp task shared(M4)
            {
                M4 = strassen_multiply_parallel(A22, B21 - B11);
            }

#pragma omp task shared(M5)
            {
                M5 = strassen_multiply_parallel(A11 + A12, B22);
            }

#pragma omp task shared(M6)
            {
                M6 = strassen_multiply_parallel(A21 - A11, B11 + B12);
            }

#pragma omp task shared(M7)
            {
                M7 = strassen_multiply_parallel(A12 - A22, B21 + B22);
            }

#pragma omp taskwait
        }

        StrassenMatrix C11(new_power), C12(new_power), C21(new_power),
            C22(new_power);

#pragma omp parallel sections
        {
#pragma omp section
            {
                C11 = M1 + M4 - M5 + M7;
            }
#pragma omp section
            {
                C12 = M3 + M5;
            }
#pragma omp section
            {
                C21 = M2 + M4;
            }
#pragma omp section
            {
                C22 = M1 - M2 + M3 + M6;
            }
        }

        StrassenMatrix result(A.m_power);

        auto insert_quadrant =
            [](StrassenMatrix &result, const StrassenMatrix &quadrant,
               std::size_t start_x, std::size_t start_y, std::size_t size) {
#pragma omp parallel for collapse(2)
                for (std::size_t y = 0; y < size; ++y) {
                    for (std::size_t x = 0; x < size; ++x) {
                        result(start_x + x, start_y + y) = quadrant(x, y);
                    }
                }
            };

#pragma omp parallel sections
        {
#pragma omp section
            {
                insert_quadrant(result, C11, 0, 0, half_size);
            }
#pragma omp section
            {
                insert_quadrant(result, C12, half_size, 0, half_size);
            }
#pragma omp section
            {
                insert_quadrant(result, C21, 0, half_size, half_size);
            }
#pragma omp section
            {
                insert_quadrant(result, C22, half_size, half_size, half_size);
            }
        }

        return result;
    }
};

} // namespace matrixes
