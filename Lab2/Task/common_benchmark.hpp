#pragma once

#include <chrono>

template <void bench_func()> class CommonBenchmark {
  public:
    CommonBenchmark(unsigned trials_count = 100)
        : m_trials_count(trials_count) {}

    long get_avg_time() {
        long sum_time = 0;

        for (unsigned trial = 0; trial < m_trials_count; ++trial)
            sum_time += measure_time();

        return (sum_time / m_trials_count);
    }

  private:
    long measure_time() {
        using clock_t = std::chrono::high_resolution_clock;
        using time_t = std::chrono::nanoseconds;

        clock_t::time_point start = clock_t::now();
        bench_func();
        clock_t::time_point end = clock_t::now();

        return std::chrono::duration_cast<time_t>(end - start).count();
    }

    unsigned m_trials_count = 0;
};
