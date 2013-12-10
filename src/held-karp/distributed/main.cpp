#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <utility>
#include <vector>

#include <stdint.h>

#include "common/location.h"
#include "common/mpi/mpi.h"
#include "tableau.h"
#include "type.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::min;
using std::numeric_limits;

void printUsage(char* progname);
void held_karp(char* locfile, int numlocs);

Mpi* mpi;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  mpi = new Mpi(&argc, &argv, 0, MPI_COMM_WORLD);

  held_karp(argv[1], atoi(argv[2]));

  mpi->finalize();
  delete mpi;
}

void held_karp(char* locfile, int numlocs) {
  DistanceMatrix* dist = DistanceMatrix::fromLocfile(locfile, numlocs);

  Tableau tableau(dist);

  if (mpi->isRoot()) {
    tableau.debugPrint();

    integer min_cost = numeric_limits<integer>::max();
    int last_row = tableau.numRows()-1;
    int num_cols = tableau.numCols();
    for (int i = 0; i < num_cols; ++i)
      min_cost = min(min_cost, tableau.data()[last_row][i] + dist->at(0, i+1));
    cout << "Min cost path: " << min_cost << endl;
  }
}

void printUsage(char* progname) {
  cerr << "Usage: " << progname << " filename numLocations" << endl;
  exit(-1);
}
