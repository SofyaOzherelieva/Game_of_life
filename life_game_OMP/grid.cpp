#include "grid.h"


Matrix::Matrix(size_t n, size_t m) : n(n), m(m), iter_(0) {
  if (n && m) {
    matrix_.resize(n);
    for (auto   &row: matrix_) {
      row.resize(m);
    }
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < m; ++j) {
        matrix_[i][j] = rand() % 2;
      }
    }
  }
}


Matrix::Matrix(std::string path_to_csv) : iter_(0) {
  std::ifstream file(path_to_csv);
  assert(file.is_open());

  std::string line;
  while (getline(file, line)) {
    matrix_.emplace_back();
    for (char c: line) {
      if (c == ',') { continue; }
      matrix_.back().push_back(c == '*');
    }
  }
  file.close();
  n = matrix_.size();
  if (n > 1) {
    m = matrix_.front().size();
    for (const auto &row: matrix_) {
      assert(m == row.size());
    }
  }
}

size_t Matrix::up(size_t i) const {
  return (i + n - 1) % n;
}

size_t Matrix::down(size_t i) const {
  return (i + 1) % n;
}

size_t Matrix::left(size_t i) const {
  return (i + m - 1) % m;
}

size_t Matrix::right(size_t i) const {
  return (i + 1) % m;
}

bool Matrix::is_alive(size_t i, size_t j) const {
  int count = matrix_[up(i)][j] + matrix_[up(i)][left(j)] + matrix_[up(i)][right(j)] + matrix_[i][left(j)] +
              matrix_[i][right(j)] + matrix_[down(i)][left(j)] + matrix_[down(i)][j] + matrix_[down(i)][right(j)];
  if (matrix_[i][j])
    return count == 2 || count == 3;
  else
    return count == 3;
}

size_t Matrix::get_row_num() const {
  return n;
}


size_t Matrix::get_column_num() const {
  return m;
}


void Matrix::set_cell(size_t i, size_t j, bool value) {
  matrix_[i][j] = value;
}


size_t Matrix::get_iter() const {
  return iter_;
}


void Matrix::set_iter(size_t iter_count) {
  iter_ = iter_count;
}

void Matrix::print_game() {
  for (int i = 0; i < m + 2; ++i)
    std::cout << '-';
  std::cout << '\n';
  for (int i = 0; i < n; ++i) {
    std::cout << '|';
    for (int j = 0; j < m; ++j)
      std::cout << (matrix_[i][j] ? 'o' : '.');
    std::cout << "|\n";
  }
  for (int i = 0; i < m + 2; ++i)
    std::cout << '-';
  std::cout << '\n';
}
