#include "tableau.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

#include "common/location.h"

using std::cout;
using std::endl;
using std::min;
using std::numeric_limits;
using std::vector;

namespace {
  void debugPrintBitset(uint32_t bitset, uint32_t num_rows) {
    cout << "{0";
    for (uint32_t row = 0; row < num_rows; ++row) {
      if (bitset & (1 << row))
        cout << "," << row+1;
    }
    cout << "}" << "\t";
  }
}

Tableau::Tableau(const vector<Location>& locs) : Tableau(new DistanceMatrix(locs)) {
}

Tableau::Tableau(DistanceMatrix* dist) : dist_(dist) {
  int num_locs = dist_->num_locs();
  num_rows_ = 1 << (num_locs-1);  // 2^(n-1) rows
  num_cols_ = num_locs - 1;       // n-1 cols

  data_ = (uint32_t**) calloc(num_rows_, sizeof(uint32_t*));
  for (uint32_t i = 0; i < num_rows_; ++i)
    data_[i] = (uint32_t*) calloc(num_cols_, sizeof(uint32_t));

  fill();
}

// static
Tableau Tableau::fromDistMatrixFile(const char* filename) {
  return Tableau(new DistanceMatrix(filename));
}

Tableau::~Tableau() {
  for (uint32_t i = 0; i < num_rows_; ++i)
    free(data_[i]);
  free(data_);

  delete dist_;
}

void Tableau::fill() {
  // Skip row 0 - corresponds to the set with only {0} (meaningless)
  for (uint32_t bitset = 1; bitset < num_rows_; ++bitset) {
    for (uint32_t endloc = 0; endloc < num_cols_; ++endloc) {
      // Skip meaningless cells, where the ith bit is not set in bitset
      // Remember that i=0 corresponds to the 0th bit of bitset, which
      // corresponds to location 1 (we've optimized out location 0 from
      // both the row bitset and the column locations).
      if (!(bitset & (1 << endloc)))
        continue;

      // "Base" case: when only 1 bit is set in bitset, the cell's value is
      // simply the distance from 0 to the location denoted by endloc (the set
      // bit).
      if (__builtin_popcount(bitset) == 1) {
        data_[bitset][endloc] = dist_->at(0, endloc+1);
      }

      // C(S,endloc) = min {
      //    C(S-{endloc},loc) + dist(loc,endloc)
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
        uint32_t min_path = numeric_limits<uint32_t>::max();
        uint32_t bitset_minus_endloc = bitset & ~(1 << endloc);

        // Here, loc=0 corresponds to location 1, so trivially we satisfy loc != 0
        for (uint32_t loc = 0; loc < num_cols_; ++loc) {
          if (loc != endloc && (bitset & (1 << loc))) {
            int loc_to_endloc = dist_->at(loc+1, endloc+1);            // dist(loc,endloc)
            uint32_t origin_to_loc = data_[bitset_minus_endloc][loc];  // C(S-{endloc},loc)
            min_path = min(min_path, origin_to_loc + loc_to_endloc);
          }
        }

        data_[bitset][endloc] = min_path;
      }
    }
  }
}

void Tableau::debugPrint() {
  for (uint32_t row = 1; row < num_rows_; ++row) {
    debugPrintBitset(row, num_rows_);
    for (uint32_t col = 0; col < num_cols_; ++col)
      cout << data_[row][col] << " ";
    cout << endl;
  }
}
