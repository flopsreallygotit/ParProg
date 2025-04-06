#include <mpi.h>

#include <cmath>
#include <iostream>

double calculate_pi(int start, int end, int total_steps) {
    double sum = 0.0;
    double step = 1.0 / total_steps;

    for (int i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 /
               (1.0 + x * x); // \pi = \int_0^1 \frac{4}{1 + x^2} dx, получается
                              // из arctg и его интегрального представления
    }

    return sum * step;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv); // Инитим MPI

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD,
                  &world_size); // Определяет общее количество процессов в
                                // коммуникаторе (он обьединяет все процессы)
    MPI_Comm_rank(MPI_COMM_WORLD,
                  &world_rank); // Определяет ранг текущего процесса

    const int total_steps = 2147483647; // Общее количество шагов интегрирования
    int steps_per_process = total_steps / world_size;

    // Определяем диапазон для каждого процесса
    int start = world_rank * steps_per_process;
    int end = (world_rank == world_size - 1) ? total_steps
                                             : start + steps_per_process;

    // Каждый процесс вычисляет свою часть
    double partial_sum = calculate_pi(start, end, total_steps);

    // Собираем результаты на процессе 0
    double global_sum;
    MPI_Reduce(&partial_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD); // Собирает данные со всех процессов и применяет
                                // к ним операцию - здесь это SUM

    if (!world_rank)
        std::cout << "Calculated Pi: " << global_sum << "\nActual Pi: " << M_PI
                  << "\nError: " << fabs(global_sum - M_PI) << std::endl;

    MPI_Finalize(); // Говорим MPI пока-пока

    return 0;
}
