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
  /**
  if (argc != 3)
    printUsage(argv[0]);

  int numLocs = atoi(argv[2]);
  vector<Location> locs(numLocs);
  readLocs(argv[1], numLocs, &locs);
  */

  Tableau tableau = Tableau::fromDistMatrixFile(argv[1]);
  tableau.debugPrint();
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
