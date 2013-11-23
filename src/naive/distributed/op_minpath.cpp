#include "op_minpath.h"

#include <mpi.h>

// static
MPI_Op _op_minpath::op_;

// static
bool _op_minpath::initialized_ = false;

// static
MPI_Op _op_minpath::getOp() {
  if (initialized_)
    return op_;
  
  MPI_Op_create(opImpl, true, &op_);
  initialized_ = true;
}

// static
void _op_minpath::opImpl(void* in, void* inout, int* len, MPI_Datatype* datatype) {
  
}
