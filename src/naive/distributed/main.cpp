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
#include "utils.h"

using namespace std;

void printUsage(char* progname);
void badProblemSize(int num_routes, int num_procs);
void readLocs(char* filename, int num_locs, vector<Location>* locs);

Mpi* mpi;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  mpi = new Mpi(&argc, &argv, 0, MPI_COMM_WORLD);
  int numLocs = atoi(argv[2]);
  int num_routes = numRoutes(numLocs);

  if (num_routes % mpi->size != 0)
    badProblemSize(num_routes, mpi->size);

  vector<Location> locs(numLocs);
  readLocs(argv[1], numLocs, &locs);

  // Rearrange the vector to the appropriate permutation.
  int routes_per_node = num_routes / mpi->size;
  permuteVector(locs, routes_per_node * mpi->rank);

  pair<int, vector<Location>> best(numeric_limits<int>::max(), vector<Location>());
  for (int i = 0; i < routes_per_node; ++i) {
    int cost = pathCost(locs);
    if (cost < best.first)
      best = make_pair(cost, vector<Location>(locs));
    next_permutation(locs.begin(), locs.end());
  }

  cout << "Shortest path: ";
  for (Location loc : best.second)
    cout << loc.id << " ";
  cout << best.first << endl;
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

void readLocs(char* filename, int numLocs, vector<Location>* locs) {
  ifstream infile(filename);
  for (int i = 0; i < numLocs; ++i)
    infile >> locs->at(i).id >> locs->at(i).x >> locs->at(i).y;
}

