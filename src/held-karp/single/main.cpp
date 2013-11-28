#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#include "common/location.h"

using namespace std;

void printUsage(char* progname);
void readLocs(char* filename, int numLocs, vector<Location>* locs);

int** makeDistTable(const vector<Location>& locs);
int** initializeTableau(int num_locs);

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  int numLocs = atoi(argv[2]);
  vector<Location> locs(numLocs);
  readLocs(argv[1], numLocs, &locs);

  int** dist = makeDistTable(locs);
  int** tableau = initializeTableau(locs.size());
}

void printUsage(char* progname) {
  cerr << "Usage: " << progname << " filename numLocations" << endl;
  exit(-1);
}

void readLocs(char* filename, int numLocs, vector<Location>* locs) {
  ifstream infile(filename);
  for (int i = 0; i < numLocs; ++i)
    infile >> locs->at(i).id >> locs->at(i).x >> locs->at(i).y;
}

// n*n distances, but only fill the upper right triangle. The diagonal is
// trivially all zeroes.
int** makeDistTable(const vector<Location>& locs) {
  int num_locs = locs.size();

  int** dist = (int**) malloc(num_locs * sizeof(int*));
  for (int i = 0; i < num_locs; ++i)
    dist[i] = (int*) malloc(num_locs * sizeof(int));

  for (int i = 0; i < num_locs-1; ++i) {
    for (int j = i+1; j < num_locs; ++j)
      dist[i][j] = locs[i].to(locs[j]);
  }

  return dist;
}

// Initialize the main tableau, where tableau[i][j] represents the minimum cost
// from location 0 to location j, visiting every location in location set i
// (represented as a bitset). Both location 0 and location j must of course
// exist in i.
//
// Given num_locs, the tableau has a known size:
//
// Each row corresponds to a subset of all locations, where each subset has the
// 0th element, and no empty set is necessary. There are 2^n subsets for some
// set of size n, therefore, there are 2^(n-1) - 1 rows in the tableau (n-1 due
// to the 0th element always being present, and -1 for lack of empty set).
//
// Each column represents an ending location, where the location is not the 0th
// element, and the location exists in the corresponding subset of locations.
// Therefore, there are (n-1) columns, although not every cell makes sense
// (e.g. the intersection of row "0,1,2" and column "3" is nonsensical, because
// no path consisting of locations 0, 1, and 2 ends at location 3.
//
// The location subsets are represented as a bitmask, and the ending locations
// are simply raw integers.
//
// Below is an example tableau for a four-location problem, where an X denotes
// a nonsensical cell. The empty cells are to be filled in using dynamic
// programming.
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
int** initializeTableau(int num_locs) {
  int num_rows = (1 << (num_locs-1)) - 1; // 2^(n-1) - 1
  int num_cols = num_locs - 1;

  int** tableau = (int**) malloc(num_rows * sizeof(int*));
  for (int i = 0; i < num_rows; ++i)
    tableau[i] = (int*) malloc(num_cols * sizeof(int));
  return tableau;
}
