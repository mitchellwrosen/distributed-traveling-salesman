#include "distance_matrix.h"

#include <cstdlib>
#include <fstream>
#include <vector>

#include "common/location.h"

using std::ifstream;
using std::vector;

DistanceMatrix::DistanceMatrix(const vector<Location>& locs) {
  num_locs_ = locs.size();

  data_ = (int**) malloc(num_locs_ * sizeof(int*));
  for (int i = 0; i < num_locs_; ++i)
    data_[i] = (int*) malloc(num_locs_ * sizeof(int));

  for (int i = 0; i < num_locs_-1; ++i) {
    for (int j = i+1; j < num_locs_; ++j)
      data_[i][j] = locs[i].to(locs[j]);
  }
}

DistanceMatrix::DistanceMatrix(const char* filename) {
  ifstream infile(filename);
  infile >> num_locs_;

  data_ = (int**) malloc(num_locs_ * sizeof(int*));
  for (int row = 0; row < num_locs_-1; ++row) {
    data_[row] = (int*) malloc(num_locs_ * sizeof(int));
    for (int col = row+1; col < num_locs_; ++col)
      infile >> data_[row][col];
  }
}

DistanceMatrix::~DistanceMatrix() {
  for (int i = 0; i < num_locs_; ++i)
    free(data_[i]);
  free(data_);
}

int DistanceMatrix::at(int row, int col) {
  if (row == col)
    return 0;
  if (row < col)
    return data_[row][col];
  return data_[col][row];
}
