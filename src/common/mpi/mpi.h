#ifndef COMMON_MPI_MPI_H_
#define COMMON_MPI_MPI_H_

#include <mpi.h>

class Mpi {
 public:
  Mpi(int* argc, char*** argv, int root, MPI_Comm comm);

  bool isRoot() const;
  void finalize();

  int size;
  int rank;
  int root;
  MPI_Comm comm;
};

#endif  // COMMON_MPI_MPI_H_
