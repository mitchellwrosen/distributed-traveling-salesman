#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point{
   int index;
   double x;
   double y;
};

typedef struct Point POINT;

float calcDistance (POINT * p1, POINT * p2);
void readData(POINT **points, FILE * fIndex);
void displayResults(int * path, int numPoints);
int * greedy (POINT ** points, int numPoints);
int findClosest (POINT * origin, POINT ** others);

int main(int argc, char** argv) {
   FILE * pFile;
   POINT **points;
   int *path;
   char *fileName;
   int numPoints;

   /* Read in values */
   fileName = argv[1];
   numPoints = strtol(argv[2], NULL, 10);
   path = malloc(sizeof(int) * numPoints);
   points = malloc(sizeof(POINT *) * numPoints);

   pFile = fopen(fileName, "r");
   readData(points, pFile);
   displayResults(greedy(points, numPoints), numPoints);

}

void displayResults(int * path, int numPoints) {
   printf("Order of vertices for path: ");
   for(int i = 0; i < numPoints; i++) {
      printf("%d ", path[i]);
   }
}

float calcDistance (POINT * p1, POINT * p2) {
   return sqrt(pow((p2->x - p1->x),2) + pow((p2->y - p1->y),2));
}

int findClosest (POINT * origin, POINT ** others) {
   int shortestDist = calcDistance(origin, *others);
   int shortestIndex = 0;
   int curIndex = 2;
   float newDist;
   others++;

   while(others++) {
      newDist = calcDistance(origin, *others);

      if(newDist < shortestDist) {
         shortestDist = newDist;
         shortestIndex = curIndex;
      }
      curIndex++;
   }

   return shortestIndex;
}

int existsIn (int * nums, int toFind) {
   while(*nums++) {
      if(*nums == toFind) {
         return 1;
      }
   }
   return 0;
}

int * greedy (POINT ** points, int numPoints) {
   int *path = malloc(sizeof(int) * numPoints);
   POINT ** neighbors = calloc(sizeof(POINT *), numPoints);
   path[0] = 0;
   int curVertex = 0;
   int neighborIndex = 0;
   /* Initialize unvisited points */
   int * unvisited = malloc(sizeof(int) * numPoints);
   for(int i=1; i < numPoints; i++) {
      unvisited[i] =  i;
   }

   /* For every vertex in "points", not in "path", calculate
   find the minimum distance from curVertex to that vertex */
   for(int curVert = 0; curVert < numPoints; curVert++) {
      if (!existsIn(path, curVert)) {
         neighbors[neighborIndex++] = points[curVert];
      }
   }

   return path;
}

void readData(POINT ** points, FILE * fIndex) {
   int pointIndex = 0;
   double pointX = 0.0;
   double pointY = 0.0;

   while(fscanf(fIndex, "%d %lf %lf", &pointIndex, &pointX, &pointY) != EOF) {
      POINT * aPoint = malloc(sizeof(POINT));
      aPoint->x = pointX;
      aPoint->y = pointY;
      aPoint->index = pointIndex;
      points[pointIndex - 1] = aPoint;
   }
}