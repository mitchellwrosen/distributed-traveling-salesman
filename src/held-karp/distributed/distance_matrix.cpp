#include "distance_matrix.h"

#include <cstdlib>
#include <fstream>
#include <vector>

#include "common/location.h"

using std::ifstream;
using std::vector;

namespace {
  vector<Location> readLocs(const char* filename, int num_locs) {
    vector<Location> locs(num_locs);
    ifstream infile(filename);
    for (int i = 0; i < num_locs; ++i)
      infile >> locs.at(i).id >> locs.at(i).x >> locs.at(i).y;
    return locs;
  }
}

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

// static
DistanceMatrix* DistanceMatrix::fromLocfile(const char* locfile, int numlocs) {
  vector<Location> locs = readLocs(locfile, numlocs);
  return new DistanceMatrix(locs);
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
