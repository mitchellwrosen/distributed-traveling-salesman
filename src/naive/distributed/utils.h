#ifndef UTILS_H_
#define UTILS_H_

#include <cstdlib>
#include <iterator>

#include <stdint.h>

#include "common/location.h"

using namespace std;

uint64_t fact(int n);
uint64_t numRoutes(int numLocs);

/**
 * Calculate the ith permutation of a collection of n elements, without
 * calculating the previous.
 *
 * Taken from http://stackoverflow.com/questions/7918806/finding-n-th-permutation-without-computing-others
 */
uint64_t* ithPermutation(const int n, uint64_t i);

/**
 * Permute elems to the specified permutation. Returns a new permuted array and
 * frees the original, so this should be called as:
 *
 *    foo = permute(foo, 5, 10);
 */
template<class T>
T* permute(T* elems, int len, uint64_t permutation) {
  uint64_t* order = ithPermutation(len, permutation);

  T* permuted = new T[len];
  for (int i = 0; i < len; ++i)
    permuted[i] = elems[order[i]];

  delete[] order;
  delete[] elems;
  return permuted;
}

template<class T>
void swap(T* elems, int x, int y) {
  T temp = elems[x];
  elems[x] = elems[y];
  elems[y] = temp;
}

#endif  // UTILS_H_
