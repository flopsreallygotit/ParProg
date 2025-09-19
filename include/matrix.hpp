#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace matrixes {

template <typename T> class StrassenMatrix {
    std::vector<T> intra;
    std::size_t length;

  public:
    StrassenMatrix(std::size_t n) {
        length = std::pow(2, n);
        intra.resize(length * length);
    }

    std::size_t get_length() const { return length; }

    void randomize() {
        std::generate(intra.begin(), intra.end(), []() { return std::rand(); });
    }

    T &operator()(std::size_t x, std::size_t y) {
        if (x >= length || y >= length)
            throw std::out_of_range("Matrix indices out of range");

        return intra[length * y + x];
    }
};

} // namespace matrixes
