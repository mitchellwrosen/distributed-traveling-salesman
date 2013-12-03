#include "common/mpi/mpi.h"

#include <mpi.h>

#include "common/mpi/utils.h"

Mpi::Mpi(int* argc, char*** argv, int root, MPI_Comm comm) 
    : size(0)  // Will be overridden
    , rank(0)  // Will be overridden
    , root(root)
    , comm(comm) 
    {
  MPI_CHECK(MPI_Init(argc, argv));

  MPI_CHECK(MPI_Comm_size(comm, const_cast<int*>(&size)));
  MPI_CHECK(MPI_Comm_rank(comm, const_cast<int*>(&rank)));
}

bool Mpi::isRoot() const {
  return rank == root;
}

void Mpi::finalize() {
  MPI_CHECK(MPI_Finalize());
}

void Mpi::ibsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Request* request) {
  MPI_CHECK(MPI_Ibsend(buf, count, datatype, dest, tag, comm, request));
}

void Mpi::ibsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag) {
  MPI_Request req;
  MPI_CHECK(MPI_Ibsend(buf, count, datatype, dest, tag, comm, &req));
  MPI_Request_free(&req);
}

void Mpi::recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Status* status) {
  MPI_CHECK(MPI_Recv(buf, count, datatype, source, tag, comm, status));
}

void Mpi::recv(void* buf, int count, MPI_Datatype datatype, int source, int tag) {
  MPI_CHECK(MPI_Recv(buf, count, datatype, source, tag, comm, MPI_STATUS_IGNORE));
}

void Mpi::recvInt(void* buf, int source, int tag, MPI_Status* status) {
  MPI_CHECK(MPI_Recv(buf, 1, MPI_INT, source, tag, comm, status));
}

void Mpi::recvInt(void* buf, int source, int tag) {
  MPI_CHECK(MPI_Recv(buf, 1, MPI_INT, source, tag, comm, MPI_STATUS_IGNORE));
}

void Mpi::reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op) {
  MPI_CHECK(MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm));
}

void Mpi::gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype) {
  MPI_CHECK(MPI_Gather(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm));
}
