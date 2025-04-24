#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class SequentialAccess {
  public:
    SequentialAccess(unsigned threads_num = 2) : m_threads(threads_num) {
        std::cout << "Initial value of access point is `" << m_access_point
                  << "`;\n";
    }

    void run() {
        for (unsigned thread_idx = 0; thread_idx < m_threads.size();
             ++thread_idx)
            m_threads[thread_idx] =
                std::thread{increment, std::ref(m_access_point), thread_idx};

        for (unsigned thread_idx = 0; thread_idx < m_threads.size();
             ++thread_idx)
            m_threads[thread_idx].join();
    }

  private:
    static void increment(int &access_point, const unsigned thread_idx) {
        std::unique_lock<std::mutex> my_lock{};
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        ++access_point;
        std::cout << "Current value of access point is `" << access_point
                  << "` in `" << thread_idx << "` thread;\n";
    }

    int m_access_point = 0;

    std::vector<std::thread> m_threads;
};

int main() {
    SequentialAccess accessor(std::thread::hardware_concurrency());
    accessor.run();

    return 0;
}
