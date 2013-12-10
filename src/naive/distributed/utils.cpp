#include "utils.h"

#include <stdint.h>

static uint64_t* factEuclidianDivision(const int n, uint64_t i);
static uint64_t* makeFactArray(const int n);
static void adjustEuclidianDivisionPerm(uint64_t* perm, const int n);

uint64_t fact(int n) {
  uint64_t ret = 1;
  for (int i = 1; i <= n; ++i)
    ret *= i;
  return ret;
}

uint64_t numRoutes(int numLocs) {
  return fact(numLocs) / numLocs;
}


uint64_t* ithPermutation(const int n, uint64_t i) {
  uint64_t* perm = factEuclidianDivision(n, i);
  adjustEuclidianDivisionPerm(perm, n);
  return perm;
}

uint64_t* factEuclidianDivision(const int n, uint64_t i) {
  uint64_t* fact = makeFactArray(n);
  uint64_t* perm = new uint64_t[n];

  for (int k = 0; k < n; ++k) {
    perm[k] = i / fact[n-1-k];
    i %= fact[n-1-k];
  }

  //delete[] fact;
  return perm;
}

uint64_t* makeFactArray(const int n) {
  int k = 0;
  uint64_t* fact = new uint64_t[n];
  fact[k] = 1;
  while (++k < n)
    fact[k] = fact[k-1] * k;
  return fact;
}

// readjust values to obtain the permutation
// start from the end and check if preceding values are lower
void adjustEuclidianDivisionPerm(uint64_t* perm, const int n) {
  for (int k = n - 1; k > 0; --k) {
    for (int j = k - 1; j >= 0; --j) {
      if (perm[j] <= perm[k])
        perm[k]++;
    }
  }
}
