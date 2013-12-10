#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <utility>

#include "common/location.h"
#include "common/mpi/mpi.h"
#include "common/mpi/utils.h"
#include "common/path.h"
#include "op_minpath.h"
#include "utils.h"

using namespace std;

void printUsage(char* progname);
void badProblemSize(uint64_t num_routes, int num_procs);
Location* readLocs(char* filename, int num_locs);

Mpi* mpi;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  mpi = new Mpi(&argc, &argv, 0, MPI_COMM_WORLD);

  int num_locs = atoi(argv[2]);
  uint64_t num_routes = numRoutes(num_locs);

  if (num_routes % mpi->size != 0)
    badProblemSize(num_routes, mpi->size);

  Location* locs = readLocs(argv[1], num_locs);

  // Rearrange the vector to the appropriate permutation.
  uint64_t routes_per_node = num_routes / mpi->size;
  locs = permute(locs, num_locs, routes_per_node * mpi->rank);
  #ifdef DEBUG
  {
     Path initial_path = Path(locs, num_locs);
     cout << "Initial path on node " << mpi->rank << ": ";
     initial_path.print();
  }
  #endif

  Path local_best_path = Path::longestPath();
  for (uint64_t i = 0; i < routes_per_node; ++i) {
    Path path = Path(locs, num_locs);
    if (path.cost() < local_best_path.cost())
      local_best_path = path;
    next_permutation(locs, locs + num_locs);
  }

  #ifdef DEBUG
  local_best_path.print();
  #endif

  char* serialized_local_best_path = local_best_path.serialize();
  int serialized_len = local_best_path.serializedLen();

  // Reduce best path of them all onto root.
  char* serialized_best_path;
  if (mpi->isRoot())
    serialized_best_path = new char[serialized_len];
  mpi->reduce(serialized_local_best_path, serialized_best_path, serialized_len, MPI_CHAR, MPI_MINPATH);

  delete[] serialized_local_best_path;

  if (mpi->isRoot()) {
    Path best_path = Path::deserialize(serialized_best_path);
    cout << "Best path: ";
    best_path.print();
  }

  mpi->finalize();
  delete mpi;
}

void printUsage(char* progname) {
  cerr << "Usage: " << progname << " filename numLocations" << endl;
  exit(-1);
}

void badProblemSize(uint64_t num_routes, int num_procs) {
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
