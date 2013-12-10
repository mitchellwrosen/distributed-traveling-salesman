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
  // from http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
  integer count_bits(integer n) {
    integer c; // c accumulates the total bits set in v
    for (c = 0; n; c++)
      n &= n - 1; // clear the least significant bit set
    return c;
  }

  void debugPrintBitset(integer bitset, integer num_rows) {
    cout << "{0";
    for (integer row = 0; row < num_rows; ++row) {
      if (bitset & (1 << row))
        cout << "," << row+1;
    }
    cout << "}" << "\t";
  }
}

Tableau::Tableau(DistanceMatrix* dist) {
  int num_locs = dist->num_locs();
  num_rows_ = 1 << (num_locs-1);  // 2^(n-1) rows
  num_cols_ = num_locs - 1;       // n-1 cols

  data_ = (integer**) calloc(num_rows_, sizeof(integer*));
  from_ = (int**) calloc(num_rows_, sizeof(int*));
  for (integer i = 0; i < num_rows_; ++i) {
    data_[i] = (integer*) calloc(num_cols_, sizeof(integer));
    from_[i] = (int*) calloc(num_cols_, sizeof(int));
  }

  fill(dist);
}

Tableau::~Tableau() {
  for (integer i = 0; i < num_rows_; ++i)
    free(data_[i]);
  free(data_);
}

void Tableau::fill(DistanceMatrix* dist) {
  // Skip row 0 - corresponds to the set with only {0} (meaningless)
  for (integer bitset = 1; bitset < num_rows_; ++bitset) {
    for (integer endloc = 0; endloc < num_cols_; ++endloc) {
      // Skip meaningless cells, where the ith bit is not set in bitset
      // Remember that i=0 corresponds to the 0th bit of bitset, which
      // corresponds to location 1 (we've optimized out location 0 from
      // both the row bitset and the column locations).
      if (!(bitset & (1 << endloc)))
        continue;

      // "Base" case: when only 1 bit is set in bitset, the cell's value is
      // simply the distance from 0 to the location denoted by endloc (the set
      // bit).
      if (count_bits(bitset) == 1) {
        data_[bitset][endloc] = dist->at(0, endloc+1);
        from_[bitset][endloc] = -1;
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
        integer min_path = numeric_limits<integer>::max();
        integer bitset_minus_endloc = bitset & ~(1 << endloc);
        int from = -1;

        // Here, loc=0 corresponds to location 1, so trivially we satisfy loc != 0
        for (integer loc = 0; loc < num_cols_; ++loc) {
          if (loc != endloc && (bitset & (1 << loc))) {
            int loc_to_endloc = dist->at(loc+1, endloc+1);             // dist(loc,endloc)
            integer origin_to_loc = data_[bitset_minus_endloc][loc];  // C(S-{endloc},loc)

            integer min_path_candidate = origin_to_loc + loc_to_endloc;
            if (min_path_candidate < min_path) {
              min_path = min_path_candidate;
              from = loc;
            }
          }
        }

        data_[bitset][endloc] = min_path;
        from_[bitset][endloc] = from;
      }
    }
  }
}

void Tableau::debugPrint() {
  #ifdef DEBUG
  for (integer row = 1; row < num_rows_; ++row) {
    debugPrintBitset(row, num_rows_);
    for (integer col = 0; col < num_cols_; ++col) {
      if (!(row & (1 << col))) {
        cout << "X ";
      } else {
        cout << data_[row][col];
        debugPrintPath(row, col);
      }
    }
    cout << endl;
  }
  #endif
}

void Tableau::debugPrintPath(integer row, integer col) {
  int num_locs = count_bits(row) + 1;
  int* locs = new int[num_locs];

  integer bitset = row;
  integer endloc = col;
  for (int i = num_locs-1; i >= 0; --i) {
    locs[i] = endloc;

    integer old_bitset = bitset;

    // Adjust bitset to exclude endloc.
    bitset = bitset & ~(((integer) 1) << endloc);

    // Adjust endloc, per the breadcrumbs in from_.
    endloc = from_[old_bitset][endloc];
  }

  cout << "(";
  for (int i = 0; i < num_locs; ++i)
    cout << locs[i]+1 << ",";
  cout << ") ";

  delete locs;
}
