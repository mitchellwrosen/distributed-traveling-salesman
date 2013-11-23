#include "common/mpi/mpi.h"

#include <mpi.h>

#include "common/mpi/utils.h"

Mpi::Mpi(int* argc, char*** argv, int root, MPI_Comm comm) : root(root), comm(comm) {
  MPI_CHECK(MPI_Init(argc, argv));

  MPI_CHECK(MPI_Comm_size(comm, &size));
  MPI_CHECK(MPI_Comm_rank(comm, &rank));
}

bool Mpi::isRoot() const {
  return rank == root;
}

void Mpi::finalize() {
  MPI_CHECK(MPI_Finalize());
}
