#include <cstdlib>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>

#include "common_benchmark.hpp"

static void get_pipe_fds(int &read_fd, int &write_fd) {
    int pipe_fd[] = {0, 0};

    if (pipe(pipe_fd) == -1) {
        std::cerr << "Can't create pipe!\n";

        exit(-1);
    }

    read_fd = pipe_fd[0];
    write_fd = pipe_fd[1];
}

static void pipe_communicator() {
    constexpr size_t max_buf_size = 256;
    char buffer[max_buf_size] = {};

    int read_fd = 0, write_fd = 0;
    get_pipe_fds(read_fd, write_fd);

    pid_t cur_pid = fork();

    if (cur_pid == -1) {
        std::cerr << "Can't do fork!\n";

        return;
    }

    else if (cur_pid == 0) { // New process
        const std::string message{"MSG"};

        close(read_fd);
        ssize_t bytes_write =
            write(write_fd, message.c_str(), message.length() + 1);
        close(write_fd);

#ifdef DEBUG
        std::cerr << "Bytes sent `" << bytes_write << "`;\n";
#endif

        exit(0);
    }

    close(write_fd);
    ssize_t bytes_read = read(read_fd, buffer, max_buf_size);
    close(read_fd);

#ifdef DEBUG
    std::cerr << "Readed message is `" << buffer << "`;\n";
#endif
}

int main() {
    CommonBenchmark<pipe_communicator> benchmark{};

    long avg_time = benchmark.get_avg_time();

    std::cout << "Average process communication time is `" << avg_time
              << "` ns;\n";

    return 0;
}
