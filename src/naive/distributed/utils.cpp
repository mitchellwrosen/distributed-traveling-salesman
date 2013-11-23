#include "utils.h"

uint64_t fact(int n) {
  uint64_t ret = 1;
  for (int i = 1; i <= n; ++i)
    ret *= i;
  return ret;
}

uint64_t numRoutes(int numLocs) {
  return fact(numLocs) / numLocs / 2;
}

int pathCost(const vector<Location>& path) {
  int cost = 0;

  const auto end = path.end()-1;
  for (auto iter = path.begin(); iter != end; ++iter)
    cost += iter->to(*(iter+1));
  cost += path.back().to(path.front());

  return cost;
}

vector<size_t> ithPermutation(const int n, int i) {
  int j, k = 0;

  vector<int> fact(n);
  vector<size_t> perm(n);

  // compute factorial numbers
  fact[k] = 1;
  while (++k < n)
    fact[k] = fact[k - 1] * k;

  // compute factorial code
  for (k = 0; k < n; ++k) {
    perm[k] = i / fact[n - 1 - k];
    i = i % fact[n - 1 - k];
  }

  // readjust values to obtain the permutation
  // start from the end and check if preceding values are lower
  for (k = n - 1; k > 0; --k) {
    for (j = k - 1; j >= 0; --j) {
      if (perm[j] <= perm[k])
        perm[k]++;
    }
  }

  return perm;
}
