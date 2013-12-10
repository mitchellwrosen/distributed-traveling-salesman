#ifndef COMMON_MPI_LOGGING_H_
#define COMMON_MPI_LOGGING_H_

#ifdef DEBUG
#include <cstdio>
#define LOG(...) { fprintf(stderr, "Node %d: ", mpi->rank); fprintf(stderr, __VA_ARGS__); }
#else
#define LOG(...) ;
#endif

#endif  // COMMON_MPI_LOGGING_H_
