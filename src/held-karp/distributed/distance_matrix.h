#ifndef DISTANCE_MATRIX_H_
#define DISTANCE_MATRIX_H_

#include <vector>

#include "common/location.h"

using std::vector;

class DistanceMatrix {
 public:
  DistanceMatrix(const vector<Location>& locs);
  
  static DistanceMatrix* fromLocfile(const char* locfile, int numlocs);

  ~DistanceMatrix();

  int at(int row, int col);

  // Getters
  int num_locs() const { return num_locs_; }

 private:
  int num_locs_;
  int** data_;
};

#endif  // DISTANCE_MATRIX_H_
