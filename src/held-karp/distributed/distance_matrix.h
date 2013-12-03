#ifndef DISTANCE_MATRIX_H_
#define DISTANCE_MATRIX_H_

#include <vector>

#include "common/location.h"

using std::vector;

class DistanceMatrix {
 public:
  DistanceMatrix(const vector<Location>& locs);

  /**
   * Read in a file of an upper-right distance matrix. The first integer is the
   * number of locations. For example,
   *
   *    --[1]--
   *   /   |   \
   *  /   20    \
   * 10    |    15
   *  |   [4]   |
   *  |  /   \  |
   *  | 25   30 |
   *  |/       \|
   * [2]--35---[3]
   *
   * would be encoded as:
   *
   * 4 10 15 20 35 25 30
   */
  DistanceMatrix(const char* filename);

  ~DistanceMatrix();

  int at(int row, int col);

  // Getters
  int num_locs() const { return num_locs_; }

 private:
  int num_locs_;
  int** data_;
};

#endif  // DISTANCE_MATRIX_H_
