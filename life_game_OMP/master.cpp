#include "master.h"

size_t target_epoch = 0;


void change_grid(
  const Matrix &curr_grid, Matrix &next_grid,
  size_t first_row, size_t last_row) {

  for (size_t i = first_row; i != last_row; ++i) {
    for (size_t j = 0; j != curr_grid.get_column_num(); ++j) {
      next_grid.set_cell(i, j, curr_grid.is_alive(i, j));
    }
  }
}

bool is_number(const std::string &s) {
  return s.find_first_not_of("0123456789") == std::string::npos;
}

void Master::communicate() {
  omp_init_lock(&omp_lock);

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
  is_running_ = true;


#pragma omp parallel num_threads(2)
  {
#pragma omp single nowait
    {
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

    while (is_running_) {
      while (grid_.get_iter() < target_epoch) {

        size_t n = grid_.get_row_num();

        if (num_threads > n) {
          num_threads = n;
        }

        size_t x    = n / num_threads;
        size_t y    = n % num_threads;
        size_t curr = 0;

        size_t curr_iter = next_grid_.get_iter();

#pragma omp parallel for num_threads(y)
        for (int i = 0; i < y; i++) {
          change_grid(grid_, next_grid_, curr, curr + x + 1);
          curr = curr + x + 1;
        }
#pragma omp parallel for num_threads(num_threads-y)
        for (int i = y; i < num_threads; i++) {
          change_grid(grid_, next_grid_, curr, curr + x);
          curr = curr + x;
        }

        omp_set_lock(&omp_lock);

        next_grid_.set_iter(curr_iter + 1);
        if (grid_.get_iter() < next_grid_.get_iter()) {
          grid_ = next_grid_;
        }

        omp_unset_lock(&omp_lock);
      }
    }
  }
}


void Master::premature_slaves_stop() {
  omp_set_lock(&omp_lock);
  if(grid_.get_iter() < target_epoch){
    target_epoch = grid_.get_iter() + 1;
  }
  omp_unset_lock(&omp_lock);
}


void Master::status() {
  omp_set_lock(&omp_lock);
  std::cout << "Current iteration: " << grid_.get_iter() << "\n";
  grid_.print_game();
  omp_unset_lock(&omp_lock);
}


void Master::run(size_t n) {
  omp_set_lock(&omp_lock);
  target_epoch += n;
  omp_unset_lock(&omp_lock);
}

void Master::stop() {
  premature_slaves_stop();
  std::cout << "Current iteration: " << target_epoch << "\n";
}


void Master::quit() {
  premature_slaves_stop();
  is_running_ = false;
  std::cout << "The end.\n";
}