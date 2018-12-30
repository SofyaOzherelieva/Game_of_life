cmake_minimum_required(VERSION 3.9)
project(life_game_OMP)

set(CMAKE_CXX_STANDARD 17)

add_executable(life_game_OMP main.cpp grid.h grid.cpp
        master.h master.cpp)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17 -Wall -Wextra -fopenmp -lrt")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(life_game_OMP Threads::Threads)
