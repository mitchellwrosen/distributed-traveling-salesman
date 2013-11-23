#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <vector>

#include "common/location.h"

using std::vector;

uint64_t fact(int n);
uint64_t numRoutes(int numLocs);
int pathCost(const vector<Location>& path);

#endif  // UTILS_H_
