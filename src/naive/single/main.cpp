#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#include "common/location.h"
#include "utils.h"

using namespace std;

void printUsage(char* progname);
void readLocs(char* filename, int numLocs, vector<Location>* locs);

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  int numLocs = atoi(argv[2]);
  vector<Location> locs(numLocs);
  readLocs(argv[1], numLocs, &locs);

  pair<int, vector<Location> > best(numeric_limits<int>::max(), vector<Location>());
  int num_routes = numRoutes(numLocs);
  for (int i = 0; i < num_routes; ++i) {
    int cost = pathCost(locs);
    if (cost < best.first)
      best = make_pair(cost, vector<Location>(locs));
    next_permutation(locs.begin(), locs.end());
  }

  cout << "Shortest path: ";
  vector<Location>::const_iterator end = best.second.end();
  for (vector<Location>::const_iterator iter = best.second.begin(); iter != end; ++iter)
    cout << iter->id << " ";
  cout << best.first << endl;
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
