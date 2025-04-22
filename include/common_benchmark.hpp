#pragma once

#include <chrono>
#include <utility>

template <auto Func, typename... Args> class CommonBenchmark final {
  public:
    CommonBenchmark(unsigned trials_count = 100)
        : m_trials_count(trials_count) {}

    long get_avg_time(Args &...args) {
        using clock_t = std::chrono::high_resolution_clock;
        using time_t = std::chrono::nanoseconds;

        long sum_time = 0;
        for (unsigned trial = 0; trial < m_trials_count; ++trial) {
            clock_t::time_point start = clock_t::now();
            Func(std::forward<Args>(args)...);
            clock_t::time_point end = clock_t::now();

            sum_time += std::chrono::duration_cast<time_t>(end - start).count();
        }

        return (sum_time / m_trials_count);
    }

  private:
    unsigned m_trials_count = 0;
};
