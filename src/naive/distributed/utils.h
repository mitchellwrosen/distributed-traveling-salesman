#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <vector>

#include<iostream>

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
size_t* ithPermutation(const int n, int i);

template<class T>
void permute(T* elems, int len, int permutation) {
  size_t* order = ithPermutation(len, permutation);

  for (int s = 1, d; s < len; ++s) {
    for (d = order[s]; d < s; d = order[d])
      ;

    if (d == s) {
      while (d = order[d], d != s)
        swap(elems, s, d);
    }
  }

  delete[] order;
}

template<class T>
void swap(T* elems, int x, int y) {
  T temp = elems[x];
  elems[x] = elems[y];
  elems[y] = temp;
}

#endif  // UTILS_H_
