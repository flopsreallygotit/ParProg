#include <iostream>
#include <thread>

#include "benchmark.hpp"

struct StringBuffer {
    bool m_modified = false;
    std::string m_content;

    StringBuffer(size_t length) : m_content(length, 0) {}

    void write(const std::string &message) {
        m_modified = true;
        m_content = message;
    }

    std::string read() { return m_content; }
};

static void speak(StringBuffer *buffer) {
    const std::string message = "MSG";
    buffer->write(message);
}

static void listen(StringBuffer *buffer) {
    while (!buffer->m_modified) {
    }
}

void thread_communicator(StringBuffer *common_buffer) {
    std::thread speaker = std::thread{speak, common_buffer};
    std::thread listener = std::thread{listen, common_buffer};

    speaker.join();
    listener.join();
}

int main() {
    StringBuffer *common_buffer = new StringBuffer{256};
    benchmarks::StaticBenchmark<thread_communicator, StringBuffer *>
        benchmark{};

    long avg_time = benchmark.get_avg_time(common_buffer);

    std::cout << "Average thread communication time is `" << avg_time
              << "` ns;\n";

    delete common_buffer;

    return 0;
}
