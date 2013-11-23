#ifndef OP_MINPATH_H_
#define OP_MINPATH_H_

#include <mpi.h>

#define MPI_MINPATH op_minpath::getOp()

/**
 * Class to wrap/create a minpath reduction operation. Can't be static, because
 * this logic has to happen after MPI_Init of course. The API to create custom
 * reduction operations is so horrible :(
 */
class _op_minpath {
 public:
  // Use MPI_MINPATH instead of this.
  static MPI_Op getOp();
 private:
  static void opImpl(void* in, void* inout, int* len, MPI_Datatype* datatype);

  static MPI_Op op_;
  static bool initialized_;
};

#endif  // OP_MINPATH_H_
