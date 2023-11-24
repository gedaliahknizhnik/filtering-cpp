cmake_minimum_required(VERSION 3.13)
project(smoothing)
set(CMAKE_CXX_STANDARD 17)

find_package (Python3 COMPONENTS Interpreter Development)

include_directories(include ${Python3_INCLUDE_DIRS})

add_executable(example src/smoothing_example.cpp)

add_executable(example_plot src/plotting_example.cpp)
target_link_libraries(example_plot Python3::Python)