#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <mpi.h>

const double t_max = 50.0; // Максимальное время
const double x_max = 60.0; // Максимальная координата

double t_step = 0.1; // Шаг по времени
const double x_step = 0.1; // Шаг по координате

const double alpha = 1.0; // Коэффициент переноса

const int ROOT = 0; // Ранг главного процесса

// Функция правой части уравнения
inline double func(double t, double x) { return x * t; }

// Начальное условие
inline double fi(double x) { return x * x * x / 12; }

// Граничное условие
inline double ksi(double t) { return t * t * t / 12; }

// Заполнение слоя по схеме "левый уголок"
void fill_layer(int knot_t, int knot_x, std::vector<std::vector<double>> &u) {
    double CFL = alpha * t_step / x_step;

    u[knot_t + 1][knot_x] = u[knot_t][knot_x] -
                            CFL * (u[knot_t][knot_x] - u[knot_t][knot_x - 1]) +
                            t_step * func(knot_t * t_step, knot_x * x_step);
}

// Вывод результатов в файл
void output_solution(int num_knots_t, int num_knots_x,
                     const std::vector<std::vector<double>> &u) {
    std::ofstream results{"results.csv", std::ofstream::out | std::ofstream::trunc};
    results << "x,t,u\n";

    for (int knot_t = 0; knot_t < num_knots_t; ++knot_t) {
        double t = knot_t * t_step;

        for (int knot_x = 0; knot_x < num_knots_x; ++knot_x) {
            double x = knot_x * x_step;

            results << x << "," << t << "," << u[knot_t][knot_x] << "\n";
        }
    }
}

void output_statistics(int size, double exec_time) {
    std::ofstream statistics{"statistics.csv", std::ios::app};
    statistics << size << "," << exec_time << "," << t_step << "\n";
}

// Инициализация сетки
void init_grid(int num_knots_x, int num_knots_t,
               std::vector<std::vector<double>> &u) {
    // Инициализация по времени (t=0)
    for (int m = 0; m < num_knots_x; ++m)
        u[0][m] = fi(m * x_step);

    // Граничное условие (x=0)
    for (int k = 0; k < num_knots_t; ++k)
        u[k][0] = ksi(k * t_step);
}

// Параллельное решение
void parallel_solution(int rank, int size, int num_knots_x, int num_knots_t,
                       std::vector<std::vector<double>> &u) {
    // Распределение точек по процессам
    int part = num_knots_x / size;
    int shift = num_knots_x % size;

    int num_knots = (rank == ROOT) ? part + shift : part;
    int x_start = (rank == ROOT) ? 0 : part * rank + shift;
    int x_end = x_start + num_knots;

    // Основной цикл по времени
    for (int knot_t = 0; knot_t < num_knots_t - 1; ++knot_t) {
        // Вычисление внутренних точек
        for (int knot_x = x_start + 1; knot_x < x_end; ++knot_x)
            fill_layer(knot_t, knot_x, u);

        // Обмен граничными точками
        if (size > 1) {
            if (rank > 0) {
                MPI_Send(&u[knot_t + 1][x_start], 1, MPI_DOUBLE, rank - 1, 0,
                         MPI_COMM_WORLD);
                MPI_Recv(&u[knot_t + 1][x_start - 1], 1, MPI_DOUBLE, rank - 1,
                         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            if (rank < size - 1) {
                MPI_Send(&u[knot_t + 1][x_end - 1], 1, MPI_DOUBLE, rank + 1, 0,
                         MPI_COMM_WORLD);
                MPI_Recv(&u[knot_t + 1][x_end], 1, MPI_DOUBLE, rank + 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Сбор результатов на главном процессе
    if (rank == ROOT)
        for (int r = 1; r < size; ++r) {
            int first = part * r + shift;
            int last = first + ((r == size - 1) ? part : part);

            MPI_Recv(&u[num_knots_t - 1][first], last - first, MPI_DOUBLE, r, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

    else
        MPI_Send(&u[num_knots_t - 1][x_start], num_knots, MPI_DOUBLE, ROOT, 0,
                 MPI_COMM_WORLD);
}

// Последовательное решение
void serial_solution(int num_knots_t, int num_knots_x,
                     std::vector<std::vector<double>> &u) {
    for (int knot_t = 0; knot_t < num_knots_t - 1; ++knot_t)
        for (int knot_x = 1; knot_x < num_knots_x; ++knot_x)
            fill_layer(knot_t, knot_x, u);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_knots_x = static_cast<int>(x_max / x_step);
    int num_knots_t = static_cast<int>(t_max / t_step);

    // Изменение шага по времени переменной среды
    std::string t_step_env = getenv("T_STEP");
    if (t_step_env.size())
        t_step = std::stod(t_step_env);

    // Инициализация сетки
    std::vector<std::vector<double>> u(num_knots_t,
                                       std::vector<double>(num_knots_x));
    init_grid(num_knots_x, num_knots_t, u);

    double start_time = MPI_Wtime();

    if (size > 1)
        parallel_solution(rank, size, num_knots_x, num_knots_t, u);
    else
        serial_solution(num_knots_t, num_knots_x, u);

    double end_time = MPI_Wtime();

    if (rank == ROOT) {
        output_solution(num_knots_t, num_knots_x, u);
        output_statistics(size, end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}
