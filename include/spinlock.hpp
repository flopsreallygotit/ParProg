#include <atomic>
#include <thread>

class SpinLock final {
  public:
    SpinLock() {}

    void lock() {
        m_retries = 0;
        while (flag.test_and_set(std::memory_order_acquire)) {
            backoff();
            ++m_retries;
        }
    }

    void unlock() { flag.clear(std::memory_order_release); }

  private:
    void backoff() {
        if (m_retries < max_retries)
            std::this_thread::yield();
        else {
            auto delay =
                std::chrono::microseconds(1 << (m_retries - max_retries));
            std::this_thread::sleep_for(delay);
        }
    }

    std::atomic_flag flag = ATOMIC_FLAG_INIT;

    unsigned int m_retries{0};
    static constexpr const unsigned int max_retries = 8;
};
