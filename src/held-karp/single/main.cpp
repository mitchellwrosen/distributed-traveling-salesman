#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#include "common/location.h"
#include "tableau.h"

using namespace std;

void printUsage(char* progname);
void readLocs(char* filename, int numLocs, vector<Location>* locs);

int** makeDistTable(const vector<Location>& locs);
uint32_t** initializeTableau(int num_locs);

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage(argv[0]);

  int numLocs = atoi(argv[2]);
  vector<Location> locs(numLocs);
  readLocs(argv[1], numLocs, &locs);

  int** dist = makeDistTable(locs);
  Tableau tableau(locs.size());
  tableau.fill(dist, locs.size());

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
