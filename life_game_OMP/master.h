#pragma once

#include <cassert>
#include <vector>
#include <omp.h>

#include "grid.h"


class Master {
  public:
    Master() : grid_(), next_grid_(),
               is_running_(false) {}

    void communicate();

  private:
    void premature_slaves_stop();

    void status();

    void run(size_t n);

    void stop();

    void quit();

    Matrix     grid_;
    Matrix     next_grid_;
    bool       is_running_;
    omp_lock_t omp_lock;
};