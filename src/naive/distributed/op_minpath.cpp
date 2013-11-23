#include "op_minpath.h"

#include <mpi.h>

#include "common/path.h"

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
  return op_;
}

// static
void _op_minpath::opImpl(void* in, void* inout, int* len, MPI_Datatype* datatype) {
  Path pathOne = Path::deserialize((char*) in);
  Path pathTwo = Path::deserialize((char*) inout);

  // Replace "inout" if necessary, which is when "in" is a shorter path.
  if (pathOne.cost() < pathTwo.cost()) {
    char* serialized = pathOne.serialize();
    memcpy(inout, serialized, pathOne.serializedLen());
  }
}
