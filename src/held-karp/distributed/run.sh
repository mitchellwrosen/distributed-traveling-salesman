#!/bin/bash

LOCS=$1
PROCS=`expr $LOCS - 1`
EXE="mpirun -mca btl_tcp_if_include eth0 -n $PROCS -hostfile hosts ./held-karp-distributed ./static/$LOCS.txt $LOCS"

if (( $# != 1 )); then
   echo "Usage: ./held-karp-distributed numLocs"
   exit 1
fi

(time $EXE) > results 2>&1

