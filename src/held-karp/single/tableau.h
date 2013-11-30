#ifndef TABLEAU_H_
#define TABLEAU_H_

#include <cstdint>

#define MAX_LOCATIONS 20

class Tableau {
 public:
  // Initialize the main tableau, where tableau[i][j] represents the minimum cost
  // from location 0 to location j, visiting every location in location set i
  // (represented as a bitset). Both location 0 and location j must of course
  // exist in i.
  //
  // Below is an example tableau for a four-location problem, where an X denotes
  // a nonsensical cell. The empty cells are to be filled in using dynamic
  // programming.
  //
  //
  //             +---+---+---+
  //             | 1 | 2 | 3 |
  // +-----------+---+---+---+
  // | {0,1}     |   | X | X |
  // +-----------+---+---+---+
  // | {0,2}     | X |   | X |
  // +-----------+---+---+---+
  // | {0,3}     | X | X |   |
  // +-----------+---+---+---+
  // | {0,1,2}   |   |   | X |
  // +-----------+---+---+---+
  // | {0,1,3}   |   | X |   |
  // +-----------+---+---+---+
  // | {0,2,3}   | X |   |   |
  // +-----------+---+---+---+
  // | {0,1,2,3} |   |   |   |
  // +-----------+---+---+---+
  //
  // Because every set contains location 0, we need only allocate 2^(n-1) rows
  // (row 0, corresponding to the empty set, will simply never be used). Thus,
  // a bitset-to-index mapping is simply a right-shift. For example,
  //
  //    0 1 0 0 1 1 0 1  // Set {0,2,3,6}
  //
  //      0 1 0 0 1 1 0  // Right-shift away the 0th bit, because it's implied.
  //                     // Use these bits to index into array.
  //
  explicit Tableau(int num_locs);

  ~Tableau();

  uint32_t at(uint32_t bitset, uint32_t endloc);

  // Fill in this empty tableau, given a distance matrix (upper-right).
  void fill(int** dist, int num_locs);

 private:
  int num_rows_;
  int num_cols_;

  uint32_t** data_;
};

#endif  // TABLEAU_H_
