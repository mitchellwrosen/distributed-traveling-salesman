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

void Mpi::reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op) {
  MPI_CHECK(MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm));
}

void Mpi::gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype) {
  MPI_CHECK(MPI_Gather(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm));
}
