#include "utils.h"

static size_t* factEuclidianDivision(const int n, int i);
static int* makeFactArray(const int n);
static void adjustEuclidianDivisionPerm(size_t* perm, const int n);

uint64_t fact(int n) {
  uint64_t ret = 1;
  for (int i = 1; i <= n; ++i)
    ret *= i;
  return ret;
}

uint64_t numRoutes(int numLocs) {
  return fact(numLocs) / numLocs / 2;
}


size_t* ithPermutation(const int n, int i) {
  size_t* perm = factEuclidianDivision(n, i);
  adjustEuclidianDivisionPerm(perm, n);
  return perm;
}

size_t* factEuclidianDivision(const int n, int i) {
  int* fact = makeFactArray(n);
  size_t* perm = new size_t[n];

  for (int k = 0; k < n; ++k) {
    perm[k] = i / fact[n-1-k];
    i %= fact[n-1-k];
  }

  //delete[] fact;
  return perm;
}

int* makeFactArray(const int n) {
  int k = 0;
  int* fact = new int[n];
  fact[k] = 1;
  while (++k < n)
    fact[k] = fact[k-1] * k;
  return fact;
}

// readjust values to obtain the permutation
// start from the end and check if preceding values are lower
void adjustEuclidianDivisionPerm(size_t* perm, const int n) {
  for (int k = n - 1; k > 0; --k) {
    for (int j = k - 1; j >= 0; --j) {
      if (perm[j] <= perm[k])
        perm[k]++;
    }
  }
}
