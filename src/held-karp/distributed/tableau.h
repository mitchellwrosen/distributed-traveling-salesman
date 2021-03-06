#ifndef TABLEAU_H_
#define TABLEAU_H_

#include <vector>

#include <stdint.h>

#include "common/location.h"
#include "distance_matrix.h"
#include "type.h"

#define UNINITIALIZED numeric_limits<integer>::max()

using std::vector;

class Tableau {
 public:
  // TODO Update this comment
  //
  // Initialize the main tableau, where tableau[i][j] represents the minimum cost
  // from location 0 to location j, visiting every location in location set i
  // (represented as a bitset). Both location 0 and location j must of course
  // exist in i.
  //
  // Below is an example tableau for a four-location problem, where an X denotes
  // a nonsensical cell. The empty cells are to be filled in using dynamic
  // programming.
  //
  //                 Location 1
  //                /
  //               /   Location 2
  //              /   /
  //             |   |
  //           +---+---+---+
  //           | 0 | 1 | 2 |
  // +---------+---+---+---+
  // | {}      | X | X | X |  // Location 0
  // +---------+---+---+---+
  // | {1}     |   | X | X |  // Locations 0 and 1
  // +---------+---+---+---+
  // | {2}     | X |   | X |  // Locations 0 and 2
  // +---------+---+---+---+
  // | {3}     | X | X |   |  // and so on...
  // +---------+---+---+---+
  // | {1,2}   |   |   | X |
  // +---------+---+---+---+
  // | {1,3}   |   | X |   |
  // +---------+---+---+---+
  // | {2,3}   | X |   |   |
  // +---------+---+---+---+
  // | {1,2,3} |   |   |   |
  // +---------+---+---+---+
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
  Tableau(DistanceMatrix* dist);

  ~Tableau();

  void debugPrint();

  // Getters
  integer numRows() const { return num_rows_; }
  integer numCols() const { return num_cols_; }
  integer** data() const { return data_; }

 private:
  void initializeSendbuf(int num_locs);

  void fill(DistanceMatrix* dist);
  void fillTwoElementSets(DistanceMatrix* dist);

  // Read data from elsewhere in the tableau, which will possibly recv it from
  // another node.
  integer readData(integer bitset, integer loc);

  // Write to the tableau, which will send it to all other nodes.
  void writeData(integer bitset, integer loc, integer cost);

  integer* sendbuf_;

  integer num_rows_;
  integer num_cols_;

  integer** data_;
};

#endif  // TABLEAU_H_
