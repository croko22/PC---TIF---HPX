#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/init.hpp>
#include <hpx/async.hpp>

#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <iomanip>

std::mt19937 gen;

bool is_point_in_circle()
{
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double x = dis(gen);
    double y = dis(gen);
    return (x * x + y * y <= 1.0);
}

hpx::future<int> monte_carlo_task(int num_points)
{
    int count_inside_circle = 0;
    for (int i = 0; i < num_points; ++i)
    {
        if (is_point_in_circle())
        {
            ++count_inside_circle;
        }
    }
    return hpx::make_ready_future(count_inside_circle);
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    int num_points = vm["points"].as<int>();
    int num_threads = vm["threads"].as<int>();

    gen.seed(std::random_device{}());

    auto start = std::chrono::high_resolution_clock::now();

    int points_per_task = num_points / num_threads;

    std::vector<hpx::future<int>> futures;
    for (int i = 0; i < num_threads; ++i)
    {
        futures.push_back(monte_carlo_task(points_per_task));
    }

    hpx::wait_all(futures);

    int total_points_inside_circle = 0;
    for (auto& f : futures)
    {
        total_points_inside_circle += f.get();
    }

    double estimated_area = 4.0 * total_points_inside_circle / num_points;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Estimated Area: " << estimated_area << std::endl;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    using namespace hpx::program_options;
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
    cmdline.add_options()
        ("points", value<int>()->default_value(1000000), "Number of points for Monte Carlo simulation")
        ("threads", value<int>()->default_value(4), "Number of threads to use");

    hpx::local::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}