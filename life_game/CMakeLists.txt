cmake_minimum_required(VERSION 3.9)
project(life_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(life_game main.cpp barrier.h grid.h grid.cpp
        master.h master.cpp slave.h slave.cpp stuff.h)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17 -Wall -Wextra")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(life_game Threads::Threads)
