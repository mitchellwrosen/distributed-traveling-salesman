#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <utility>
#include <vector>

#include "common/location.h"
#include "common/mpi/mpi.h"
#include "common/mpi/utils.h"
#include "common/path.h"
#include "op_minpath.h"
#include "utils.h"

using namespace std;

void printUsage(char* progname);
void badProblemSize(int num_routes, int num_procs);
Location* readLocs(char* filename, int num_locs);

Mpi* mpi;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  mpi = new Mpi(&argc, &argv, 0, MPI_COMM_WORLD);
  int num_locs = atoi(argv[2]);
  int num_routes = numRoutes(num_locs);

  if (num_routes % mpi->size != 0)
    badProblemSize(num_routes, mpi->size);

  Location* locs = readLocs(argv[1], num_locs);

  // Rearrange the vector to the appropriate permutation.
  int routes_per_node = num_routes / mpi->size;
  permute(locs, num_locs, routes_per_node * mpi->rank);

  Path best_path = Path::longestPath();
  for (int i = 0; i < routes_per_node; ++i) {
    Path path = Path(locs, num_locs);
    if (path.cost() < best_path.cost())
      best_path = path;
    next_permutation(locs, locs + num_locs);
  }

  best_path.print();

  mpi->finalize();
  delete mpi;
}

void printUsage(char* progname) {
  cerr << "Usage: " << progname << " filename numLocations" << endl;
  exit(-1);
}

void badProblemSize(int num_routes, int num_procs) {
  cerr << "Unable to distribute " << num_routes << " routes among " <<
      num_procs << " processors." << endl;
  mpiAbort(-1);
}

Location* readLocs(char* filename, int numLocs) {
  Location* locs = new Location[numLocs];
  ifstream infile(filename);
  for (int i = 0; i < numLocs; ++i)
    infile >> locs[i].id >> locs[i].x >> locs[i].y;
  return locs;
}
