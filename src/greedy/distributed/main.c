#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

struct Point{
   int index;
   double distance;
   double x;
   double y;
   struct Point *next;
};

struct Result {
   int index;
   double cost;
};

typedef struct Point POINT;
typedef struct Result RESULT;

float calcDistance (POINT * p1, POINT * p2);
POINT * readData(FILE * fIndex);
int randomInt(int min, int max);
double calcCost(POINT * path);
void displayResults(POINT * path);
POINT * greedy (POINT * points, int numPoints);
POINT * findClosest (POINT * origin, POINT * others);

int main(int argc, char** argv) {
   FILE * pFile;
   POINT *points;
   POINT *path;
   RESULT **results;
   char *fileName;
   int numPoints, procNum, cheapestIndex, incomingIndex;
   double cost, cheapestCost, incomingCost;


   /* MPI Vars */
   int ierr, num_procs, my_id;
   MPI_Status status;
   MPI_Request send_request;
   MPI_Init(&argc, &argv);

   ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
   ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

   srand(time(NULL) + my_id);
   /* Read in values */
   if(my_id != 0) {
      fileName = argv[1];
      numPoints = strtol(argv[2], NULL, 10);
      pFile = fopen(fileName, "r");
      points = readData(pFile);
      path = greedy(points, numPoints);
      cost = calcCost(path);
      MPI_Send(&(path->index), 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
      MPI_Send(&cost, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
   } else {
      results = malloc(sizeof(RESULT *) * num_procs);
      cheapestCost = 0;
      for(procNum = 1; procNum < num_procs; procNum++) {
         MPI_Recv(&incomingIndex, 1, MPI_INT, procNum, 1, MPI_COMM_WORLD, &status);
         MPI_Recv(&incomingCost, 1, MPI_DOUBLE, procNum, 1, MPI_COMM_WORLD, &status);
         if(cheapestCost == 0 || incomingCost < cheapestCost) {
            cheapestCost = incomingCost;
            cheapestIndex = incomingIndex;
         }
      }
      printf("The best route starts on %d with a cost of %lf", cheapestIndex, cheapestCost);
   }



   ierr = MPI_Finalize();
}

double calcCost(POINT * path) {
   double cost = 0.0;
   while(path) {
      cost += path->distance;
      path = path->next;
   }
   return cost;
}

void displayResults(POINT * path) {
   double cost = 0.0;
   while(path) {
      cost += path->distance;
      path = path->next;
   }
}

float calcDistance (POINT * p1, POINT * p2) {
   return sqrt(pow((p2->x - p1->x),2) + pow((p2->y - p1->y),2));
}

POINT * findClosest (POINT * origin, POINT * others) {
   float shortestDist;
   float curDist;
   POINT * closest;
   int curIndex = 1;

   shortestDist = calcDistance(origin, others);
   closest = others;
   others = others->next;

   while(others) {
      curDist = calcDistance(origin, others);
      if(curDist < shortestDist || !shortestDist) {
         shortestDist = curDist;
         closest = others;
      }
      others = others->next;
   }
   closest->distance = shortestDist;
   return closest;
}

POINT * removePoint(POINT * points, POINT * toRemove) {
   if(points == NULL) {
      return NULL;
   }

   if(points == toRemove) {
      return points->next;
   }

   points->next = removePoint(points->next, toRemove);
   return points;
}

//http://stackoverflow.com/questions/5289613/c-generate-random-float-between-two-floats
int randomInt(int min, int max)
{
   return (rand() % max);
}

POINT * greedy (POINT * points, int numPoints) {
   POINT * path = NULL;
   POINT * pathHd = NULL;
   POINT * closestPoint;
   int startPoint;

   /* Start at random point */

   startPoint = randomInt(0, numPoints);
   printf("starting at point %d\n", startPoint);
   path = points;
   while(startPoint--) {
      path = path->next;
   }

   points = removePoint(points, path);
   pathHd = path;

   while(points) {
      closestPoint = findClosest(path, points);
      path->next = closestPoint;
      path = path->next;
      points = removePoint(points, closestPoint);
      path->next = NULL;
   }

   return pathHd;
}

POINT * readData(FILE * fIndex) {
   POINT * points = NULL;
   int pointIndex = 0;
   double pointX = 0.0;
   double pointY = 0.0;

   while(fscanf(fIndex, "%d %lf %lf", &pointIndex, &pointX, &pointY) != EOF) {
      POINT * aPoint = malloc(sizeof(POINT));
      aPoint->x = pointX;
      aPoint->y = pointY;
      aPoint->distance = 0;
      aPoint->index = pointIndex;

      if(!points) {
         aPoint->next = NULL;
         points = aPoint;
      } else {
         aPoint->next = points;
         points = aPoint;
      }
   }
   return points;
}