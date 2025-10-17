#pragma once

#include <atomic>
#include <concepts>

template <std::integral T = int> class BoundedCounter final {
  public:
    explicit BoundedCounter(T max_value)
        : m_max_value(max_value), m_current(0) {}

    // Why am I using do-while?
    // T expected = current_.load();
    // if (expected < max_value_) {
    //     current_.compare_exchange_weak(expected, expected + 1); <- At this
    //     moment another thread can change current
    // } -> https://habr.com/ru/companies/piter/articles/670456/

    [[nodiscard]] bool try_increment() noexcept {
        T expected = m_current.load(std::memory_order_acquire);

        do {
            if (expected >= m_max_value)
                return false;
        } while (!m_current.compare_exchange_weak(expected, expected + 1,
                                                  std::memory_order_release,
                                                  std::memory_order_relaxed));

        return true;
    }

    [[nodiscard]] bool try_increment_by(T value) noexcept {
        T expected = m_current.load(std::memory_order_acquire);

        do {
            T new_value = expected + value;
            if (new_value > m_max_value)
                return false;
        } while (!m_current.compare_exchange_weak(expected, expected + value,
                                                  std::memory_order_release,
                                                  std::memory_order_relaxed));

        return true;
    }

    [[nodiscard]] bool try_decrement() noexcept {
        T expected = m_current.load(std::memory_order_acquire);

        do {
            if (expected == 0)
                return false;
        } while (!m_current.compare_exchange_weak(expected, expected - 1,
                                                  std::memory_order_release,
                                                  std::memory_order_relaxed));

        return true;
    }

    [[nodiscard]] T value() const noexcept {
        return m_current.load(std::memory_order_acquire);
    }

    [[nodiscard]] T max_value() const noexcept { return m_max_value; }

    void reset() noexcept { m_current.store(0, std::memory_order_release); }

    [[nodiscard]] bool is_max() const noexcept {
        return value() >= m_max_value;
    }
    [[nodiscard]] bool is_zero() const noexcept { return value() == 0; }

  private:
    const T m_max_value;
    std::atomic<T> m_current;
};
