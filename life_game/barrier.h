#pragma once

#include <condition_variable>
#include <mutex>


class CyclicBarrier {
  public:
    explicit CyclicBarrier(const size_t num_threads)
      : num_threads_(num_threads), counter_(0) {
    }

    void pass_through() {
      std::unique_lock<std::mutex> lock(mutex_);
      counter_++;
      size_t group_num = counter_ / num_threads_;
      if (counter_ % num_threads_ == 0) {
        all_threads_arrived_.notify_all();
      } else {
        all_threads_arrived_.wait(lock, [this, group_num]() {
          return counter_ / num_threads_ > group_num;
        });
      }
    }

    void set_num_threads(size_t num_threads) {
      num_threads_ = num_threads;
    }

  private:
    size_t                  num_threads_;
    size_t                  counter_;
    std::mutex              mutex_;
    std::condition_variable all_threads_arrived_;
};
