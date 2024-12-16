cmake_minimum_required(VERSION 3.19)
project(my_hpx_project CXX)
find_package(HPX REQUIRED)

add_executable(multiplication multiplicacion_matrices.cpp)
target_link_libraries(multiplication HPX::hpx HPX::wrap_main HPX::iostreams_component)

add_executable(montecarlo montecarlo.cpp)
target_link_libraries(montecarlo HPX::hpx HPX::wrap_main HPX::iostreams_component)

add_executable(heat difusion_calor.cpp)
target_link_libraries(heat HPX::hpx HPX::wrap_main HPX::iostreams_component)