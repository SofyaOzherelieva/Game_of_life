#pragma once

#include <mutex>
#include <thread>

#include "barrier.h"
#include "stuff.h"
#include "grid.h"


class Slave {
  public:
    Slave(size_t first_row, size_t last_row, CyclicBarrier &barrier,
          Matrix &grid, Matrix &next_grid, std::mutex &grid_lock, std::condition_variable &cv);

    void run();

    void join();

  private:
    size_t                  first_row_;
    size_t                  last_row_;
    CyclicBarrier           &barrier_;
    Matrix                  &grid_;
    Matrix                  &next_grid_;
    std::mutex              &grid_lock_;
    std::thread             worker_;
    std::condition_variable &cv_;
};
