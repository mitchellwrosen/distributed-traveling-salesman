#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <utility>
#include <vector>

#include "common/location.h"
#include "common/mpi/mpi.h"
#include "tableau.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::min;
using std::numeric_limits;

void printUsage(char* progname);
vector<Location> readLocs(char* filename, int num_locs);

int** makeDistTable(const vector<Location>& locs);
uint64_t** initializeTableau(int num_locs);

Mpi* mpi;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  mpi = new Mpi(&argc, &argv, 0, MPI_COMM_WORLD);

  int num_locs = atoi(argv[2]);
  vector<Location> locs = readLocs(argv[1], num_locs);

  DistanceMatrix* dist = new DistanceMatrix(locs);

  Tableau tableau(dist);
  //tableau.debugPrint();

  uint64_t min_cost = numeric_limits<int>::max();
  int num_rows = tableau.numRows();
  int num_cols = tableau.numCols();
  for (int i = 0; i < num_cols; ++i)
    min_cost = min(min_cost, tableau.data()[num_rows-1][i] + dist->at(0, i+1));
  cout << "Min cost path: " << min_cost << endl;
}

void printUsage(char* progname) {
  cerr << "Usage: " << progname << " filename numLocations" << endl;
  exit(-1);
}

vector<Location> readLocs(char* filename, int num_locs) {
  vector<Location> locs(num_locs);
  ifstream infile(filename);
  for (int i = 0; i < num_locs; ++i)
    infile >> locs.at(i).id >> locs.at(i).x >> locs.at(i).y;
  return locs;
}
