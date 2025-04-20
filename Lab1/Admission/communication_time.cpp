#include <chrono>
#include <iostream>
#include <vector>

#include <mpi.h>

void measure_communication(int rank, int size, int data_size) {
    const int warmup_iters = 10;
    const int measure_iters = 100;
    std::vector<char> send_buf(data_size,
                               'a'); // aaaaaaaaaaaaaaaaa... ну вы поняли
    std::vector<char> recv_buf(data_size);

    // Синхронизация перед началом измерений
    MPI_Barrier(MPI_COMM_WORLD); // Каждый процесс останавливается здесь, пока
                                 // ВСЕ процессы не достигнут этого вызова

    if (rank == 0) {
        for (int i = 0; i < warmup_iters; ++i) { // Без warm-up'a зависает...
            MPI_Send(send_buf.data(), data_size, MPI_BYTE, 1, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }

        // Измерение времени
        double total_time = 0.0;
        for (int i = 0; i < measure_iters; ++i) {
            auto start =
                MPI_Wtime(); // Аналог std::chrono::high_resolution_clock,
                             // оптимизированный для mpi

            MPI_Send(send_buf.data(), data_size, MPI_BYTE, 1, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            auto end = MPI_Wtime();

            total_time += (end - start);
        }

        double avg_time = total_time / measure_iters;
        double bandwidth =
            (2.0 * data_size) / (avg_time * 1024 * 1024); // MB/s (туда-обратно)

        std::cout << "Message size: " << data_size / 1024 << " KB"
                  << ", Avg time: " << avg_time * 1e6 << " us"
                  << ", Bandwidth: " << bandwidth << " MB/s" << std::endl;
    }

    else if (rank == 1) {
        // Процесс-получатель
        for (int i = 0; i < warmup_iters + measure_iters; ++i) {
            MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Send(recv_buf.data(), data_size, MPI_BYTE, 0, 0,
                     MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (!rank)
            std::cerr << "This program requires at least 2 processes"
                      << std::endl;

        MPI_Finalize();

        return -1;
    }

    const std::vector<int> test_sizes = {
        1 * 1024,       // 1KB
        4 * 1024,       // 4KB
        16 * 1024,      // 16KB
        64 * 1024,      // 64KB
        256 * 1024,     // 256KB
        1 * 1024 * 1024 // 1MB
    };

    for (int sz : test_sizes)
        measure_communication(rank, size, sz);

    MPI_Finalize();

    return 0;
}
