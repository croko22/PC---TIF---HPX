#include <hpx/algorithm.hpp>
#include <hpx/init.hpp>
#include <hpx/parallel/algorithms/for_loop.hpp>
#include <hpx/include/parallel_executors.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

void heat_diffusion(std::vector<double>& u, std::vector<double>& u_next, int N, int iterations, double alpha)
{
    for (int iter = 0; iter < iterations; iter++)
    {
        hpx::experimental::for_loop(hpx::execution::par, 1, N - 1, [&](int i) {
            u_next[i] = u[i] + alpha * (u[i - 1] - 2 * u[i] + u[i + 1]);
        });

        hpx::experimental::for_loop(hpx::execution::par, 1, N - 1, [&](int i) {
            u[i] = u_next[i];
        });
    }
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    double alpha = 0.01;

    // std::vector<int> N_values = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};
    // std::vector<int> iterations_values = {100, 200, 300, 500, 1000, 2000, 5000};
    std::vector<int> N_values = {1000, 5000, 10000, 50000, 100000};
    std::vector<int> iterations_values = {100, 500, 1000};

    std::ofstream file("heat_results_hpx.csv");
    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }
    file << "N,iterations,execution_time\n";

    for (int N : N_values)
    {
        for (int iterations : iterations_values)
        {
            std::vector<double> u(N, 0.0);
            std::vector<double> u_next(N, 0.0);
            u[0] = 100.0;
            u[N - 1] = 100.0;

            auto start = std::chrono::high_resolution_clock::now();
            heat_diffusion(u, u_next, N, iterations, alpha);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            file << N << "," << iterations << "," << std::fixed << std::setprecision(6) << duration.count() << "\n";
        }
    }

    file.close();
    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    hpx::local::init_params init_args;
    return hpx::local::init(hpx_main, argc, argv, init_args);
}
