#ifndef COMMON_MPI_UTILS_H_
#define COMMON_MPI_UTILS_H_

#define MPI_CHECK(call) \
  if ((call) != MPI_SUCCESS) { \
    std::cerr << "MPI error calling \""#call"\"\n"; \
    mpiAbort(-1); \
  }

void mpiAbort(int err);

#endif  // COMMON_MPI_UTILS_H_
