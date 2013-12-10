-----------------------------
Greedy Traveling Sales Person
-----------------------------

The greedy TSP Algorithm calculates a cycle by
finding the closest point until all points are exhausted.

The distributed version simply starts on a random vertex
and performs the greedy TSP algorithm. 

To run single:

./a.out dataPath numPoints

EX:
./a.out ~/CSC569/distributed-traveling-salesman/static/100000.txt 100000

To run distributed:

./a.out -hostfile hosts -prefix /usr/lib64/openmpi -mca btl_tcp_if_include eth0 -n #procs a.out filePath

EX:
mpirun -hostfile hosts -prefix /usr/lib64/openmpi -mca btl_tcp_if_include eth0 -n 20 a.out ~/CSC569/distributed-traveling-salesman/static/100000.txt 100000