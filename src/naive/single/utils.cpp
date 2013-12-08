#include "utils.h"

uint64_t fact(int n) {
  uint64_t ret = 1;
  for (int i = 1; i <= n; ++i)
    ret *= i;
  return ret;
}

uint64_t numRoutes(int numLocs) {
  return fact(numLocs) / numLocs;
}

int pathCost(const vector<Location>& path) {
  int cost = 0;

  vector<Location>::const_iterator end = path.end()-1;
  for (vector<Location>::const_iterator iter = path.begin(); iter != end; ++iter)
    cost += iter->to(*(iter+1));
  cost += path.back().to(path.front());

  return cost;
}
