#include "master.h"

size_t target_epoch       = 0;
bool   slaves_is_running_ = true;


bool is_number(const std::string &s) {
  return s.find_first_not_of("0123456789") == std::string::npos;
}

void Master::communicate() {
  std::string command = "";
  std::string arg     = "";

  std::cout << "Initialization: 'START N M K' or " <<
            "'START file_path.csv K', where K is a number of threads\n";
  std::cin >> command;

  while (command != "START") {
    std::cout << "First command should be 'START'\n";
    std::cin >> command;
  }

  std::cin >> arg;
  if (is_number(arg)) {
    size_t n = std::stoi(arg);
    size_t m = 0;
    std::cin >> m;
    grid_ = Matrix(n, m);
  } else {
    grid_ = Matrix(arg);
  }

  next_grid_ = grid_;
  size_t num_threads = 0;
  std::cin >> num_threads;
  barrier_.set_num_threads(num_threads);
  init_slaves(num_threads);
  is_running_ = true;

  /////////////////////////////////////

  slaves_is_running_ = true;

  for (auto &slave_thread: slaves_) {
    slave_thread.run();
  }

  /////////////////////////////////////

  std::cout << "Available commands: 'STATUS', 'RUN', 'STOP', 'QUIT'\n";
  while (is_running_) {
    std::cin >> command;

    if (command == "STATUS") {
      status();
    } else if (command == "RUN") {
      size_t n = 0;
      std::cin >> n;
      run(n);
    } else if (command == "STOP") {
      stop();
    } else if (command == "QUIT") {
      quit();
    } else {
      std::cout << "Invalid command\n";
    }
  }
}


void Master::slaves_stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (grid_.get_iter() < target_epoch) {
    target_epoch = grid_.get_iter() + 1;
  }
}

void Master::premature_slaves_stop() {
  slaves_stop();
  slaves_is_running_ = false;
  cv_.notify_all();
  for (auto &slave_thread: slaves_) {
    slave_thread.join();
  }
}

void Master::init_slaves(size_t num_threads) {
  size_t n = grid_.get_row_num();

  if (num_threads > n) {
    num_threads = n;
  }

  size_t x    = n / num_threads;
  size_t y    = n % num_threads;
  size_t curr = 0;

  for (int i = 0; i < y; i++) {
    slaves_.emplace_back(curr, curr + x + 1, barrier_, grid_, next_grid_, mutex_, cv_);
    curr = curr + x + 1;
  }

  for (int i = y; i < num_threads; i++) {
    slaves_.emplace_back(curr, curr + x, barrier_, grid_, next_grid_, mutex_, cv_);
    curr = curr + x;
  }
}


void Master::status() {
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << "Current iteration: " << grid_.get_iter() << "\n";
  grid_.print_game();
}


void Master::run(size_t n) {
  mutex_.lock();
  target_epoch += n;
  mutex_.unlock();
  cv_.notify_all();
}

void Master::stop() {
  slaves_stop();
  std::cout << "Current iteration: " << target_epoch << "\n";
}


void Master::quit() {
  premature_slaves_stop();
  is_running_ = false;
  std::cout << "The end.\n";
}