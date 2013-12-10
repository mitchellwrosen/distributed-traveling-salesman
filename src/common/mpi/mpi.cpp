#include "common/mpi/mpi.h"

#include <mpi.h>

#include <stdint.h>

#include "common/mpi/logging.h"
#include "common/mpi/utils.h"

Mpi::Mpi(int* argc, char*** argv, int root, MPI_Comm comm)
    : size(0)  // Will be overridden
    , rank(0)  // Will be overridden
    , root(root)
    , comm(comm)
    , mpi(this)
    {
  MPI_CHECK(MPI_Init(argc, argv));

  MPI_CHECK(MPI_Comm_size(comm, const_cast<int*>(&size)));
  MPI_CHECK(MPI_Comm_rank(comm, const_cast<int*>(&rank)));
}

bool Mpi::isRoot() const {
  return rank == root;
}

void Mpi::barrier() {
  MPI_CHECK(MPI_Barrier(comm));
}

void Mpi::finalize() {
  MPI_CHECK(MPI_Finalize());
}

void Mpi::bufferAttach(void* buf, int size) {
  MPI_CHECK(MPI_Buffer_attach(buf, size));
}

void Mpi::bufferDetach(void* buf) {
  int junk;
  MPI_CHECK(MPI_Buffer_detach(buf, &junk));
}

void Mpi::ibsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Request* request) {
  MPI_CHECK(MPI_Ibsend(buf, count, datatype, dest, tag, comm, request));
}

void Mpi::ibsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag) {
  MPI_Request req;
  ibsend(buf, count, datatype, dest, tag, &req);
  MPI_Request_free(&req);
}

void Mpi::ibsendInt(void* buf, int dest, int tag, MPI_Request* request) {
  LOG("MPI_Ibsend { payload=%ld, dest=%d, tag=%d }\n", *((uint64_t*) buf),
      dest, tag);

  MPI_CHECK(MPI_Ibsend(buf, 1, MPI_LONG_LONG, dest, tag, comm, request));
}

void Mpi::ibsendInt(void* buf, int dest, int tag) {
  MPI_Request req;
  ibsendInt(buf, dest, tag, &req);
  MPI_Request_free(&req);
}

void Mpi::recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Status* status) {
  MPI_CHECK(MPI_Recv(buf, count, datatype, source, tag, comm, status));
}

void Mpi::recv(void* buf, int count, MPI_Datatype datatype, int source, int tag) {
  recv(buf, count, datatype, source, tag, MPI_STATUS_IGNORE);
}

void Mpi::recvInt(void* buf, int source, int tag, MPI_Status* status) {
  recv(buf, 1, MPI_LONG_LONG, source, tag, status);
}

void Mpi::recvInt(void* buf, int source, int tag) {
  LOG("MPI_Recving from source=%d,tag=%d\n", source, tag);

  recvInt(buf, source, tag, MPI_STATUS_IGNORE);

  LOG("MPI_Recv'd %ld from source=%d,tag=%d\n", *((uint64_t*) buf), source,
      tag);
}

void Mpi::reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op) {
  MPI_CHECK(MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm));
}

void Mpi::gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype) {
  MPI_CHECK(MPI_Gather(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm));
}
