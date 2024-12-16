////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2021 Dimitra Karatza
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

// Parallel implementation of matrix multiplication

#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/init.hpp>

#include <cstddef>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

std::mt19937 gen;

int hpx_main(hpx::program_options::variables_map& vm)
{
    using element_type = int;

    std::size_t const size = vm["size"].as<std::size_t>();
    std::size_t const threads = vm["threads"].as<std::size_t>();

    std::size_t const rows = size;
    std::size_t const cols = size;
    std::vector<int> A(rows * cols);
    std::vector<int> B(rows * cols);
    std::vector<int> R(rows * cols, 0);

    unsigned int seed = std::random_device{}();
    gen.seed(seed);

    int const lower = -1000;
    int const upper = 1000;
    std::uniform_int_distribution<element_type> dis(lower, upper);
    auto generator = std::bind(dis, gen);

    hpx::ranges::generate(A, generator);
    hpx::ranges::generate(B, generator);

    auto policy = hpx::execution::par.with(hpx::execution::static_chunk_size(threads));

    auto start = std::chrono::high_resolution_clock::now();
    hpx::experimental::for_loop(policy, 0, rows, [&](auto i) {
        hpx::experimental::for_loop(0, cols, [&](auto j) {
            for (std::size_t k = 0; k < cols; ++k) {
                R[i * cols + j] += A[i * cols + k] * B[k * cols + j];
            }
        });
    });
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Tiempo de multiplicación: " << elapsed.count() << " segundos" << std::endl;

    return hpx::local::finalize();
}
//]

///////////////////////////////////////////////////////////////////////////////
//[mul_main
int main(int argc, char* argv[])
{
    using namespace hpx::program_options;
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
    // Argumentos permitidos
    cmdline.add_options()
        ("size",
        hpx::program_options::value<std::size_t>()->default_value(256),
        "Tamaño de la matriz cuadrada")
        ("threads",
        hpx::program_options::value<std::size_t>()->default_value(4),
        "Número de hilos/procesos a usar");

    hpx::local::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}
//]
