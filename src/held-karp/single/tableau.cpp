#include "tableau.h"

#include <cassert>
#include <cstdlib>

Tableau::Tableau(int num_locs) {
  assert(num_locs <= MAX_LOCATIONS);

  num_rows_ = (1 << (num_locs-1)); // 2^(n-1) rows
  num_cols_ = num_locs - 1;        // n-1 cols

  data_ = (uint32_t**) calloc(num_rows_, sizeof(uint32_t*));
  for (int i = 0; i < num_rows_; ++i)
    data_[i] = (uint32_t*) calloc(num_cols_, sizeof(uint32_t));
}

Tableau::~Tableau() {
  for (int i = 0; i < num_rows_; ++i)
    free(data_[i]);
  free(data_);
}

uint32_t Tableau::at(uint32_t bitset, uint32_t endloc) {
  int row = bitset >> 1;
  int col = endloc-1;

  assert(row < num_rows_);
  assert(col < num_cols_);

  return data_[row][col];
}


void Tableau::fill(int** dist, int num_locs) {
  // Skip row 0 - corresponds to the set with only {0} (meaningless)
  for (int i = 1; i < num_rows_; ++i) {
    for (int j = 0; j < num_cols_; ++j) {
      // Skip meaningless cells, where the jth bit is not set in i
      // Remember that j=0 corresponds to the 0th bit of i, which corresponds
      // to location 1 (we've optimized out location 0 from both the row bitset
      // and the column locations).
      if (!(i & (1 << j)))
        continue;

      // "Base" case: when only 1 bit is set in i, the cell's value is simply
      // the distance from 0 to the location denoted by j (the set bit).
      if (__builtin__popcount(i) == 1) {
        data_[i][j] = dist[0][j+1];
      } 

      // C(S,j) = min {
      //    C(S-{j},k) + dist(k,j)
      // } for all k in S, k != 0, k != j
      //
      // Intuitively:
      //
      // The minimum distance from location 0 to location j, visiting each
      // location in set S, is defined as the minimum of all paths constructed
      // as such:
      //
      // For each other location not equal to 0 or j in S, denoted k, sum 
      // together the minimum path from 0 to k using the same locations in S
      // (less j), plus the distance from k to j.
      else {
        // TODO
      }
    }
  }
}
