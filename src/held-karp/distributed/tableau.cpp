#include "tableau.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>

#include "common/location.h"
#include "common/mpi/mpi.h"
#include "common/mpi/utils.h"

using std::cout;
using std::cerr;
using std::endl;
using std::min;
using std::numeric_limits;
using std::vector;

extern Mpi* mpi;

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

Tableau::Tableau(DistanceMatrix* dist) {
  int num_locs = dist->num_locs();
  num_rows_ = 1 << (num_locs-1);  // 2^(n-1) rows
  num_cols_ = num_locs - 1;       // n-1 cols

  // For simplicity, require that each node fills one column.
  if (num_cols_ != (uint32_t) mpi->size) {
    cerr << "Cannot create tableau with " << num_cols_ << " cols using " 
        << mpi->size << " nodes." << endl;
    mpiAbort(-1);
  }

  data_ = (uint32_t**) malloc(num_rows_ * sizeof(uint32_t*));
  for (uint32_t i = 0; i < num_rows_; ++i) {
    data_[i] = (uint32_t*) malloc(num_cols_ * sizeof(uint32_t));
    memset(data_+i, UNINITIALIZED, num_cols_);
  }

  fill(dist);
}

// static
Tableau Tableau::fromDistMatrixFile(const char* filename) {
  return Tableau(new DistanceMatrix(filename));
}

Tableau::~Tableau() {
  for (uint32_t i = 0; i < num_rows_; ++i)
    free(data_[i]);
  free(data_);
}

void Tableau::fill(DistanceMatrix* dist) {
  fillTwoElementSets(dist);

  uint32_t endloc = mpi->rank;

  for (uint32_t bitset = num_cols_+1; bitset < num_rows_; ++bitset) {
    if (!(bitset & (1 << endloc)))
      continue;

    uint32_t min_path = numeric_limits<uint32_t>::max();
    uint32_t bitset_minus_endloc = bitset & ~(1 << endloc);

    for (uint32_t loc = 0; loc < num_cols_; ++loc) {
      if (loc != endloc && (bitset & (1 << loc))) {
        int loc_to_endloc = dist->at(loc+1, endloc+1);               // dist(loc,endloc)
        uint32_t origin_to_loc = readData(bitset_minus_endloc, loc); // C(S-{endloc},loc)
        min_path = min(min_path, origin_to_loc + loc_to_endloc);
      }
    }

    writeData(bitset, endloc, min_path); 
  }
}

void Tableau::fillTwoElementSets(DistanceMatrix* dist) {
  for (uint32_t i = 0; i < num_cols_; ++i)
    data_[i+1][i] = dist->at(0, i+1);
}

uint32_t Tableau::readData(uint32_t bitset, uint32_t loc) {
  uint32_t cost = data_[bitset][loc];
  while (cost == UNINITIALIZED) {
    // RECV
  }
  return cost;
}

void Tableau::writeData(uint32_t bitset, uint32_t loc, uint32_t cost) {
  data_[bitset][loc] = cost;
  // SEND
}

void Tableau::debugPrint() {
  for (uint32_t row = 1; row < num_rows_; ++row) {
    debugPrintBitset(row, num_rows_);
    for (uint32_t col = 0; col < num_cols_; ++col)
      cout << data_[row][col] << " ";
    cout << endl;
  }
}
