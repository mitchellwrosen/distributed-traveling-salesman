#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <vector>

#include "common/location.h"

using namespace std;

uint64_t fact(int n);
uint64_t numRoutes(int numLocs);
int pathCost(const vector<Location>& path);

/**
 * Calculate the ith permutation of a collection of n elements, without
 * calculating the previous.
 *
 * Taken from http://stackoverflow.com/questions/7918806/finding-n-th-permutation-without-computing-others
 */
vector<size_t> ithPermutation(const int n, int i);

/**
 * Reorder an orderable container, given a second container of indices.
 *
 * Taken from http://stackoverflow.com/questions/838384/reorder-vector-using-a-vector-of-indices
 */
//template<typename order_iterator, typename value_iterator>
//void reorder(order_iterator order_begin, order_iterator order_end, value_iterator v)  {
  //typedef typename iterator_traits<value_iterator>::value_type value_t;
  //typedef typename iterator_traits<order_iterator>::value_type index_t;
  //typedef typename iterator_traits<order_iterator>::difference_type diff_t;

  //diff_t negativeOne;

  //diff_t remaining = order_end - 1 - order_begin;
  //for (index_t s = index_t(); remaining > 0; ++ s) {
    //index_t d = order_begin[s];
    //if (d == negativeOne)
      //continue;

    //--remaining;

    //value_t temp = v[s];
    //for (index_t d2; d != s; d = d2) {
      //swap(temp, v[d]);
      //swap(order_begin[d], d2 = negativeOne);
      //--remaining;
    //}

    //v[s] = temp;
  //}
//}

template< class T >
void reorder(vector<T> &v, vector<size_t> const &order )  {
    for ( int s = 1, d; s < order.size(); ++ s ) {
      for ( d = order[s]; d < s; d = order[d] ) ;
      if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
    }
}

template<typename T>
void permuteVector(vector<T> vec, int permutation) {
  vector<size_t> order = ithPermutation(vec.size(), permutation);
  reorder(vec, order);
}

#endif  // UTILS_H_
