#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


class Matrix {
  public:
    Matrix() : n(0), m(0), iter_(0) {}

    Matrix(size_t n, size_t m);

    explicit Matrix(std::string path_to_csv);

    size_t up(size_t i) const;

    size_t down(size_t i) const;

    size_t left(size_t i) const;

    size_t right(size_t i) const;

    bool is_alive(size_t i, size_t j) const;

    size_t get_row_num() const;

    size_t get_column_num() const;

    size_t get_iter() const;

    void set_cell(size_t i, size_t j, bool value);

    void set_iter(size_t iter_count);

    void print_game();

  private:
    size_t                         n;
    size_t                         m;
    size_t                         iter_;
    std::vector<std::vector<bool>> matrix_;
};