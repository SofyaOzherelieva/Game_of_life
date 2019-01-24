#include "slave.h"


void change_grid(
  const Matrix &curr_grid, Matrix &next_grid,
  size_t first_row, size_t last_row) {

  for (size_t i = first_row; i != last_row; ++i) {
    for (size_t j = 0; j != curr_grid.get_column_num(); ++j) {
      next_grid.set_cell(i, j, curr_grid.is_alive(i, j));
    }
  }
}


static void run_slave(
  Matrix &next_grid, Matrix &grid,
  CyclicBarrier &barrier,
  size_t first_row, size_t last_row,
  std::mutex &mutex, std::condition_variable &cv) {

  while (slaves_is_running_) {
    {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock, [&grid] { return (grid.get_iter() < target_epoch) || !slaves_is_running_; });
    }

    size_t curr_iter = next_grid.get_iter();
    change_grid(grid, next_grid, first_row, last_row);
    barrier.pass_through();

    mutex.lock();

    next_grid.set_iter(curr_iter + 1);
    if (grid.get_iter() < next_grid.get_iter()) {
      grid = next_grid;
    }

    mutex.unlock();
  }
}


Slave::Slave(size_t first_row,
             size_t last_row,
             CyclicBarrier &barrier,
             Matrix &grid,
             Matrix &next_grid,
             std::mutex &grid_lock, std::condition_variable &cv)
  : first_row_(first_row), last_row_(last_row),
    barrier_(barrier),
    grid_(grid), next_grid_(next_grid),
    grid_lock_(grid_lock), cv_(cv) {
}


void Slave::run() {
  worker_ = std::thread(run_slave,
                        std::ref(grid_), std::ref(next_grid_),
                        std::ref(barrier_),
                        first_row_, last_row_,
                        std::ref(grid_lock_), std::ref(cv_));
}


void Slave::join() {
  if (worker_.joinable()) {
    worker_.join();
  }
}