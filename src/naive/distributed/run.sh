#!/bin/bash
EXE="mpirun -mca btl_tcp_if_include eth0 -n $2 -hostfile hosts ./naive-distributed ./static/$1.txt $1"

if (( $# != 2 )); then
   echo "Usage: ./naive-distributed numLocs numProcs"
   exit 1
fi

(time $EXE) >> results 2>&1

