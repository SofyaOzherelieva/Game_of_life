#pragma once

#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

#include "barrier.h"
#include "stuff.h"
#include "grid.h"
#include "slave.h"


class Master {
  public:
    Master() : grid_(), next_grid_(),
               is_running_(false), barrier_(0) {}

    void communicate();

  private:
    void slaves_stop();

    void premature_slaves_stop();

    void init_slaves(size_t num_threads);

    void status();

    void run(size_t n);

    void stop();

    void quit();

    Matrix             grid_;
    Matrix             next_grid_;
    bool               is_running_;
    CyclicBarrier      barrier_;
    std::mutex         mutex_;
    std::vector<Slave> slaves_;
    std::condition_variable cv_;
};